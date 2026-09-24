#include "../../include/services/teams_service.h"

#include "../../include/services/players_service.h"
#include "../../include/services/tournaments_service.h"

static bool filter_tournament_teams(const void* element, const void* ctx) {
    const Team* team = element;
    const uint32_t tournament_id = *(uint32_t*)ctx;

    return tournament_id == team->tournament_id;
}

static bool filter_user_teams(const void* element, const void* ctx) {
    const Team* team = element;
    const uint32_t player_id = *(uint32_t*)ctx;

    return player_id == team->user_id;
}

Team* get_tournament_teams(const AppContext* app_context, const uint32_t tournament_id, size_t* teams_count) {
    return entity_table_select(&app_context->teams_table, filter_tournament_teams, &tournament_id, teams_count);
}

size_t get_tournament_teams_quantity(const AppContext* app_context, const uint32_t tournament_id) {
   return entity_table_count(&app_context->teams_table, filter_tournament_teams, &tournament_id);
}

Team* get_user_teams(const AppContext* app_context, const uint32_t user_id, size_t* teams_count) {
    return entity_table_select(&app_context->teams_table, filter_user_teams, &user_id, teams_count);
}

bool get_specific_team(const AppContext* app_context, uint32_t tournament_id, uint32_t user_id, Team* out_team) {
    Team* teams = app_context->teams_table.data;
    for (size_t i = 0; i < app_context->teams_table.count; i++) {
        if (teams[i].tournament_id == tournament_id && teams[i].user_id == user_id) {
            *out_team = teams[i];
            return true;
        }
    }
    return false;
}

ServiceStatus set_team(AppContext* app_context, uint32_t tournament_id, const uint32_t player_ids[TEAM_SIZE]) {
    Team user_team;
    // Se l'utente non ha un Team per questo torneo, significa che non è iscritto. Si deve iscrivere prima!!
    if (!get_specific_team(app_context, tournament_id, app_context->current_user_id, &user_team)) {
        return STATUS_ERR_USER_NOT_SIGNED_UP;
    }
    Tournament selected_tournament;
    if (!get_tournament_by_id(app_context, tournament_id, &selected_tournament)) {
        return STATUS_ERR_TOURNAMENT_NOT_FOUND;
    }

    if (selected_tournament.state != TOURNAMENT_STATE_OPEN) {
        return STATUS_ERR_TOURNAMENT_NOT_OPEN;
    }

    size_t players_count;
    TennisPlayer* tournament_players = get_tournament_players(app_context, tournament_id, &players_count);
    if (players_count == SIZE_MAX) {
        return STATUS_ERR_OOM;
    }

    if (players_count == 0) {
        // Tecnicamente vero... heh
        return STATUS_ERR_PLAYER_NOT_IN_TOURNAMENT;
    }

    unsigned int credits_spent = 0;
    for (int i = 0; i < TEAM_SIZE; i++) {
        // Controlliamo lo slot sia popolato
        if (player_ids[i] <= 0) {
            free(tournament_players);
            return STATUS_ERR_TEAM_NOT_FILLED;
        }

        // Controlliamo che l'ID non sia ripetuto
        for (int j = i + 1; j < TEAM_SIZE; j++) {
            if (player_ids[i] == player_ids[j]) {
                free(tournament_players);
                return STATUS_ERR_PLAYER_DUPED;
            }
        }

        // Controlliamo che il tennista effettivamente partecipi a questo torneo, e che sia disponibile
        TennisPlayer* curr_player = NULL;
        bool is_not_available = false;
        for (size_t j = 0; j < players_count; j++) {
            if (player_ids[i] == tournament_players[j].id) {
                curr_player = &tournament_players[j];
                if (!curr_player->is_available) is_not_available = true;
                break;
            }
        }
        if (curr_player == NULL) {
            free(tournament_players);
            return STATUS_ERR_PLAYER_NOT_IN_TOURNAMENT;
        }
        if (is_not_available) {
            free(tournament_players);
            return STATUS_ERR_PLAYER_UNAVAILABLE;
        }
        credits_spent += get_player_cost(app_context, tournament_id, player_ids[i]);

        // Possiamo già modificare lo struct, tanto è una copia
        user_team.players_ids[i] = player_ids[i];
    }

    free(tournament_players);

    if (credits_spent > MAX_TEAM_BUDGET) {
        return STATUS_ERR_BUDGET_EXCEEDED;
    }

    // Tutto ok da qui - tutti i check passati
    entity_table_edit_item(&app_context->teams_table, user_team.id, &user_team);

    return STATUS_OK;
}