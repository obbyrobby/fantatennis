#include <string.h>
#include "../../include/services/tournaments_service.h"
#include "../../include/services/matches_service.h"
#include "../../include/services/teams_service.h"
#include "../../include/models/tournament.h"
#include "../../include/models/user.h"
#include "../../include/services/players_service.h"

// Contesti dei Filtri
/**
 * @brief Riservato per uso interno.
 */
typedef struct {
    const EntityTable* participations_table;
    uint32_t tennisplayer_id;
    bool ongoing_only;
} PlayerTournamentsCtx;

/**
 * @brief Riservato per uso interno.
 */
typedef struct {
    const EntityTable* participations_table;
    uint32_t tournament_id;
    bool not_in_tournament;
} TournamentPlayersCtx;

// Filtri
static bool filter_player_tournaments(const void* element, const void* ctx) {
    const Tournament* tournament = element;
    const PlayerTournamentsCtx* filter_ctx = ctx;
    const TournamentParticipation* participations = filter_ctx->participations_table->data;

    if (filter_ctx->ongoing_only && tournament->state != TOURNAMENT_STATE_ONGOING) {
        // Questo torneo non è in corso, sono stati richiesti solo quelli in corso!
        return false;
    }

    for (size_t i = 0; i < filter_ctx->participations_table->count; i++) {
        if (participations[i].tennisplayer_id == filter_ctx->tennisplayer_id && participations[i].tournament_id == tournament->id) {
            // Questa partecipazione riguarda il giocatore richiesto, e il torneo sul quale ci troviamo attualmente!
            return true;
        }
    }
    // Partecipazione non trovata
    return false;
}

static bool filter_tournaments(const void* element, const void* ctx) {
    const Tournament* tournament = element;
    const FilterTournamentState state = *(FilterTournamentState*)ctx;

    if (state == FILTER_TOURNAMENT_STATE_ALL) return true;
    return (TournamentState)state == tournament->state;
}

static bool filter_tournament_participations(const void* element, const void* ctx) {
    const TournamentParticipation* participation = element;
    const uint32_t tournament_id = *(uint32_t*)ctx;

    return participation->tournament_id == tournament_id;
}

static bool filter_tournament_players(const void* element, const void* ctx) {
    const TennisPlayer* player = element;
    const TournamentPlayersCtx* filter_ctx = ctx;
    const TournamentParticipation* participations = filter_ctx->participations_table->data;

    for (size_t i = 0; i < filter_ctx->participations_table->count; i++) {
        if (participations[i].tournament_id == filter_ctx->tournament_id && participations[i].tennisplayer_id == player->id) {
            return !filter_ctx->not_in_tournament;
        }
    }
    return filter_ctx->not_in_tournament;
}

// Funzioni
Tournament* get_player_tournaments(const AppContext* app_context, const uint32_t tennisplayer_id, size_t* participations_count) {
    PlayerTournamentsCtx ctx = { .participations_table = &app_context->participations_table, .tennisplayer_id = tennisplayer_id, .ongoing_only = false};
    return entity_table_select(&app_context->tournaments_table, filter_player_tournaments, &ctx, participations_count);
}

size_t get_player_tournaments_quantity(const AppContext* app_context, const uint32_t tennisplayer_id) {
    PlayerTournamentsCtx ctx = { .participations_table = &app_context->participations_table, .tennisplayer_id = tennisplayer_id, .ongoing_only = false};
    return entity_table_count(&app_context->tournaments_table, filter_player_tournaments, &ctx);
}

Tournament* get_player_ongoing_tournaments(const AppContext* app_context, const uint32_t tennisplayer_id, size_t* tournaments_count) {
    PlayerTournamentsCtx ctx = { .participations_table = &app_context->participations_table, .tennisplayer_id = tennisplayer_id, .ongoing_only = true};
    return entity_table_select(&app_context->tournaments_table, filter_player_tournaments, &ctx, tournaments_count);
}

size_t get_player_ongoing_tournaments_quantity(const AppContext* app_context, const uint32_t tennisplayer_id) {
    PlayerTournamentsCtx ctx = { .participations_table = &app_context->participations_table, .tennisplayer_id = tennisplayer_id, .ongoing_only = true};
    return entity_table_count(&app_context->tournaments_table, filter_player_tournaments, &ctx);
}

Tournament* get_tournaments(const AppContext* app_context, FilterTournamentState state_filter, size_t* tournaments_count) {
    return entity_table_select(&app_context->tournaments_table, filter_tournaments, &state_filter, tournaments_count);
}

TournamentParticipation* get_tournament_participations(const AppContext* app_context, const uint32_t tournament_id, size_t* participations_count) {
    return entity_table_select(&app_context->participations_table, filter_tournament_participations, &tournament_id, participations_count);
}

TennisPlayer* get_tournament_players(const AppContext* app_context, const uint32_t tournament_id, size_t* players_count) {
    TournamentPlayersCtx ctx = {.participations_table = &app_context->participations_table, .tournament_id = tournament_id, .not_in_tournament = false};
    return entity_table_select(&app_context->tennisplayers_table, filter_tournament_players, &ctx, players_count);
}

size_t get_tournament_players_quantity(const AppContext* app_context, const uint32_t tournament_id) {
    TournamentPlayersCtx ctx = {.participations_table = &app_context->participations_table, .tournament_id = tournament_id};
    return entity_table_count(&app_context->tennisplayers_table, filter_tournament_players, &ctx);
}

TennisPlayer* get_players_not_in_tournament(const AppContext* app_context, const uint32_t tournament_id, size_t* players_count) {
    TournamentPlayersCtx ctx = {.participations_table = &app_context->participations_table, .tournament_id = tournament_id, .not_in_tournament = true};
    return entity_table_select(&app_context->tennisplayers_table, filter_tournament_players, &ctx, players_count);
}

bool get_tournament_by_id(const AppContext* app_context, const uint32_t id, Tournament* tournament) {
    Tournament* item = entity_table_get_item_by_id(&app_context->tournaments_table, id);
    if (item == NULL) return false;
    if (tournament != NULL) *tournament = *item;
    return true;
}

ServiceStatus add_tournament(AppContext* app_context, Tournament* new_tournament, uint32_t* out_id) {
    if (!app_context->is_admin) {
        return STATUS_ERR_UNAUTHORIZED;
    }

    if (new_tournament->title[0] == '\0') {
        return STATUS_ERR_TITLE_EMPTY;
    }

    new_tournament->state = TOURNAMENT_STATE_OPEN;
    new_tournament->winner_id = 0;
    size_t newtournament_id = entity_table_add_item(&app_context->tournaments_table, new_tournament);
    if (newtournament_id == 0) {
        return STATUS_ERR_OOM;
    }

    if (out_id != NULL) {
        (*out_id) = newtournament_id;
    }

    return STATUS_OK;
}

ServiceStatus edit_tournament(AppContext* app_context, Tournament* edited_tournament) {
    if (!app_context->is_admin) {
        return STATUS_ERR_UNAUTHORIZED;
    }

    if (edited_tournament->title[0] == '\0') {
        return STATUS_ERR_TITLE_EMPTY;
    }

    Tournament current_tournament;
    if (!get_tournament_by_id(app_context, edited_tournament->id, &current_tournament)) {
        return STATUS_ERR_NOT_FOUND;
    }
    TournamentState previous_state = current_tournament.state;
    TournamentState new_state = edited_tournament->state;

    // Da Aperto a In Corso: lo permettiamo solo se sono stati aggiunti almeno due giocatori al Torneo
    if (previous_state == TOURNAMENT_STATE_OPEN && new_state == TOURNAMENT_STATE_ONGOING) {
        size_t participating_players = get_tournament_players_quantity(app_context, edited_tournament->id);
        if (participating_players < 2) {
            return STATUS_ERR_NOT_ENOUGH_PLAYERS;
        }
    }
    // Da In Corso ad Aperto: lo permettiamo solo se ancora non si è giocata alcuna partita
    else if (previous_state == TOURNAMENT_STATE_ONGOING && new_state == TOURNAMENT_STATE_OPEN) {
        size_t tournament_matches_count = get_tournament_matches_quantity(app_context, edited_tournament->id);
        if (tournament_matches_count > 0) {
            return STATUS_ERR_TOURNAMENT_ALREADY_STARTED;
        }
    }
    // Da In Corso a Completato: lo permettiamo solo se si è giocata almeno una partita
    else if (previous_state == TOURNAMENT_STATE_ONGOING && new_state == TOURNAMENT_STATE_COMPLETED) {
        size_t tournament_matches_count = get_tournament_matches_quantity(app_context, edited_tournament->id);
        if (tournament_matches_count == 0) {
            return STATUS_ERR_TOURNAMENT_NOT_STARTED;
        }

        // Inoltre, deve anche essere impostato un vincitore!
        if (edited_tournament->winner_id == 0 || edited_tournament->winner_id >= app_context->tennisplayers_table.next_id) {
            return STATUS_ERR_WINNER_NOT_SET;
        }

        // E il vincitore deve essere un partecipante del torneo!

        // Vediamo tutti i tornei a cui partecipa il vincitore specificato;
        // Se non c'è il torneo che stiamo attualmente modificando, ci è stato fornito un vincitore invalido
        size_t tournaments_count;
        bool found = false;
        Tournament* winner_tournaments = get_player_tournaments(app_context, edited_tournament->winner_id, &tournaments_count);
        if (tournaments_count == SIZE_MAX && winner_tournaments == NULL) {
            return STATUS_ERR_OOM;
        }
        for (size_t i = 0; i < tournaments_count; i++) {
            if (winner_tournaments[i].id == edited_tournament->id) {
                // Ecco il nostro torneo
                found = true;
                break;
            }
        }
        free(winner_tournaments);
        if (!found) {
            return STATUS_ERR_INVALID_WINNER;
        }
    }
    // Da Completato a In Corso: per ora lo ammettiamo, nel caso in cui un admin chiude un torneo per sbaglio
    else if (previous_state == TOURNAMENT_STATE_COMPLETED && new_state == TOURNAMENT_STATE_ONGOING) {
        // Niente, per ora
    } else if (previous_state != new_state) {
        // Tutte le altre transizioni di stato sono illegali!
        return STATUS_ERR_ILLEGAL_TRANSITION;
    }

    // STATUS_ERR_NOT_FOUND già gestito sopra
    entity_table_edit_item(&app_context->tournaments_table, edited_tournament->id, edited_tournament);

    return STATUS_OK;
}

ServiceStatus delete_tournament(AppContext* app_context, uint32_t tournament_id) {
    if (!app_context->is_admin) {
        return STATUS_ERR_UNAUTHORIZED;
    }

    if (!get_tournament_by_id(app_context, tournament_id, NULL)) {
        return STATUS_ERR_TOURNAMENT_NOT_FOUND;
    }

    // Eliminiamo a cascata tutte le partite, le squadre e le partecipazioni legate al torneo
    // che stiamo eliminando
    size_t matches_count;
    size_t teams_count;
    size_t participations_count;

    Match* tournament_matches = get_tournament_matches(app_context, tournament_id, &matches_count);
    if (matches_count == SIZE_MAX) {
        return STATUS_ERR_OOM;
    }

    Team* tournament_teams = get_tournament_teams(app_context, tournament_id, &teams_count);
    if (teams_count == SIZE_MAX) {
        if (tournament_matches != NULL) free(tournament_matches);
        return STATUS_ERR_OOM;
    }

    TournamentParticipation* tournament_participations = get_tournament_participations(app_context, tournament_id, &participations_count);
    if (participations_count == SIZE_MAX) {
        if (tournament_matches != NULL) free(tournament_matches);
        if (tournament_teams != NULL) free(tournament_teams);
        return STATUS_ERR_OOM;
    }

    entity_table_batch_begin(&app_context->matches_table);
    entity_table_batch_begin(&app_context->teams_table);
    entity_table_batch_begin(&app_context->participations_table);
    for (uint32_t i = 0; i < matches_count; i++) {
        entity_table_delete_item(&app_context->matches_table, tournament_matches[i].id);
    }

    for (uint32_t i = 0; i < teams_count; i++) {
        entity_table_delete_item(&app_context->teams_table, tournament_teams[i].id);
    }

    for (uint32_t i = 0; i < participations_count; i++) {
        entity_table_delete_item(&app_context->participations_table, tournament_participations[i].id);
    }
    entity_table_batch_commit(&app_context->matches_table);
    entity_table_batch_commit(&app_context->teams_table);
    entity_table_batch_commit(&app_context->participations_table);

    free(tournament_matches);
    free(tournament_teams);
    free(tournament_participations);
    entity_table_delete_item(&app_context->tournaments_table, tournament_id);
    return STATUS_OK;
}

ServiceStatus add_tournament_participation(AppContext* app_context, const uint32_t player_id, const uint32_t tournament_id) {
    if (!app_context->is_admin) {
        return STATUS_ERR_UNAUTHORIZED;
    }

    TennisPlayer player;
    Tournament tournament;

    // Tennista non trovato
    if (!get_player_by_id(app_context, player_id, &player)) {
        return STATUS_ERR_PLAYER_NOT_FOUND;
    }
    // Torneo non trovato
    if (!get_tournament_by_id(app_context, tournament_id, &tournament)) {
        return STATUS_ERR_TOURNAMENT_NOT_FOUND;
    }
    // Non possiamo aggiungere un Tennista ad un torneo se non è disponibile
    if (!player.is_available) {
        return STATUS_ERR_PLAYER_UNAVAILABLE;
    }
    // Non possiamo aggiungere un giocatore ad un torneo se il torneo è iniziato
    if (tournament.state != TOURNAMENT_STATE_OPEN) {
        return STATUS_ERR_TOURNAMENT_NOT_OPEN;
    }

    size_t count;
    Tournament* player_tournaments = get_player_tournaments(app_context, player_id, &count);
    if (count == SIZE_MAX) {
        return STATUS_ERR_OOM;
    }

    for (size_t i = 0; i < count; i++) {
        if (player_tournaments[i].id == tournament_id) {
            free(player_tournaments);
            return STATUS_ERR_PLAYER_ALREADY_PARTICIPATING;
        }
    }

    TournamentParticipation new_participation = { .tournament_id = tournament_id, .tennisplayer_id = player_id };
    if (entity_table_add_item(&app_context->participations_table, &new_participation) == 0) {
        free(player_tournaments);
        return STATUS_ERR_OOM;
    }
    free(player_tournaments);
    return STATUS_OK;
}

ServiceStatus delete_tournament_participation(AppContext* app_context, const uint32_t player_id, uint32_t const tournament_id) {
    if (!app_context->is_admin) {
        return STATUS_ERR_UNAUTHORIZED;
    }

    Tournament tournament;
    if (!get_tournament_by_id(app_context, tournament_id, &tournament)) {
        return STATUS_ERR_TOURNAMENT_NOT_FOUND;
    }
    // Non ci serve accedere ai dati del giocatore, basta che sappiamo se esiste o meno
    if (!get_player_by_id(app_context, player_id, NULL)) {
        return STATUS_ERR_PLAYER_NOT_FOUND;
    }

    if (tournament.state != TOURNAMENT_STATE_OPEN) {
        return STATUS_ERR_TOURNAMENT_NOT_OPEN;
    }

    size_t participations_count;
    TournamentParticipation* participations = get_tournament_participations(app_context, tournament_id, &participations_count);
    for (size_t i = 0; i < participations_count; i++) {
        if (participations[i].tennisplayer_id == player_id && participations[i].tournament_id == tournament_id) {
            entity_table_delete_item(&app_context->participations_table, participations[i].id);
            free(participations);
            return STATUS_OK;
        }
    }
    free(participations);
    return STATUS_ERR_NOT_FOUND;
}

ServiceStatus sign_up_tournament(AppContext* app_context, const uint32_t tournament_id) {
    if (app_context->current_user_id == 0) return STATUS_ERR_UNAUTHORIZED;

    Tournament tournament;
    if (!get_tournament_by_id(app_context, tournament_id, &tournament)) {
        return STATUS_ERR_TOURNAMENT_NOT_FOUND;
    }

    if (tournament.state != TOURNAMENT_STATE_OPEN) {
        return STATUS_ERR_TOURNAMENT_NOT_OPEN;
    }

    size_t teams_count;
    Team* user_teams = get_user_teams(app_context, app_context->current_user_id, &teams_count);
    if (teams_count == SIZE_MAX) return STATUS_ERR_OOM;

    for (size_t i = 0; i < teams_count; i++) {
        if (user_teams[i].tournament_id == tournament_id) {
            if (user_teams != NULL) free(user_teams);
            return STATUS_ERR_USER_ALREADY_SIGNED_UP;
        }
    }
    Team new_team = { 0 };
    new_team.tournament_id = tournament_id;
    new_team.user_id = app_context->current_user_id;

    if (entity_table_add_item(&app_context->teams_table, &new_team) == 0) {
        if (user_teams != NULL) free(user_teams);
        return STATUS_ERR_OOM;
    }

    if (user_teams != NULL) free(user_teams);
    return STATUS_OK;
}