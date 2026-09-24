#include "../../include/services/matches_service.h"
#include "../../include/services/tournaments_service.h"

static bool filter_tournament_matches(const void* element, const void* ctx) {
    const Match* match = element;
    const uint32_t tournament_id = *(uint32_t*)ctx;

    return match->tournament_id == tournament_id;
}

Match* get_tournament_matches(const AppContext* app_context, const uint32_t tournament_id, size_t* matches_count) {
    return entity_table_select(&app_context->matches_table, filter_tournament_matches, &tournament_id, matches_count);
}

size_t get_tournament_matches_quantity(const AppContext* app_context, const uint32_t tournament_id) {
    return entity_table_count(&app_context->matches_table, filter_tournament_matches, &tournament_id);
}

ServiceStatus add_match(AppContext* app_context, Match* new_match) {
    if (!app_context->is_admin) {
        return STATUS_ERR_UNAUTHORIZED;
    }

    if (new_match->player_results[0].player_id == new_match->player_results[1].player_id) {
        return STATUS_ERR_SAME_PLAYERS;
    }

    if (new_match->player_results[0].won_sets == new_match->player_results[1].won_sets) {
        return STATUS_ERR_TIE_NOT_ALLOWED;
    }

    Tournament tournament;
    if (!get_tournament_by_id(app_context, new_match->tournament_id, &tournament)) {
        return STATUS_ERR_TOURNAMENT_NOT_FOUND;
    }

    if (tournament.state != TOURNAMENT_STATE_ONGOING) {
        return STATUS_ERR_TOURNAMENT_NOT_ONGOING;
    }

    size_t players_count;
    TennisPlayer* tournament_players = get_tournament_players(app_context, new_match->tournament_id, &players_count);
    bool player1_found = false, player2_found = false;

    for (size_t i = 0; i < players_count; i++) {
        if (tournament_players[i].id == new_match->player_results[0].player_id) {
            player1_found = true;
        }
        if (tournament_players[i].id == new_match->player_results[1].player_id) {
            player2_found = true;
        }
    }
    free(tournament_players);

    if (!player1_found || !player2_found) {
        return STATUS_ERR_INVALID_PLAYERS;
    }


    size_t newmatch_id = entity_table_add_item(&app_context->matches_table, new_match);
    if (newmatch_id == 0) {
        return STATUS_ERR_OOM;
    }

    return STATUS_OK;
}

ServiceStatus edit_match(AppContext* app_context, Match* edited_match) {
    if (!app_context->is_admin) {
        return STATUS_ERR_UNAUTHORIZED;
    }

    if (edited_match->player_results[0].player_id == edited_match->player_results[1].player_id) {
        return STATUS_ERR_SAME_PLAYERS;
    }

    if (edited_match->player_results[0].won_sets == edited_match->player_results[1].won_sets) {
        return STATUS_ERR_TIE_NOT_ALLOWED;
    }

    Tournament tournament;
    if (!get_tournament_by_id(app_context, edited_match->tournament_id, &tournament)) {
        return STATUS_ERR_TOURNAMENT_NOT_FOUND;
    }

    if (tournament.state != TOURNAMENT_STATE_ONGOING) {
        return STATUS_ERR_TOURNAMENT_NOT_ONGOING;
    }

    size_t players_count;
    TennisPlayer* tournament_players = get_tournament_players(app_context, edited_match->tournament_id, &players_count);
    bool player1_found = false, player2_found = false;

    for (size_t i = 0; i < players_count; i++) {
        if (tournament_players[i].id == edited_match->player_results[0].player_id) {
            player1_found = true;
        }
        if (tournament_players[i].id == edited_match->player_results[1].player_id) {
            player2_found = true;
        }
    }
    if (!player1_found || !player2_found) {
        return STATUS_ERR_INVALID_PLAYERS;
    }

    free(tournament_players);
    bool result = entity_table_edit_item(&app_context->matches_table, edited_match->id, edited_match);
    if (!result) {
        return STATUS_ERR_NOT_FOUND;
    }

    return STATUS_OK;
}

ServiceStatus delete_match(AppContext* app_context, const uint32_t id) {
    if (!app_context->is_admin) {
        return STATUS_ERR_UNAUTHORIZED;
    }

    Match* match = entity_table_get_item_by_id(&app_context->matches_table, id);
    if (match == NULL) {
        return STATUS_ERR_NOT_FOUND;
    }

    Tournament tournament;
    // Se per assurdo la partita non è associata ad un torneo esistente, possiamo
    // eliminarla senza scrupoli
    if (!get_tournament_by_id(app_context, match->tournament_id, &tournament)) {
        // E non c'è bisogno di controllare l'esito qui perchè abbiamo già pescato il torneo prima
        entity_table_delete_item(&app_context->matches_table, id);
        return STATUS_OK;
    }

    if (tournament.state != TOURNAMENT_STATE_ONGOING) {
        return STATUS_ERR_TOURNAMENT_NOT_ONGOING;
    }

    entity_table_delete_item(&app_context->matches_table, id);
    return STATUS_OK;
}

bool get_match_by_id(const AppContext* app_context, const uint32_t id, Match* match) {
    Match* item = entity_table_get_item_by_id(&app_context->matches_table, id);
    if (item == NULL) return false;
    if (match != NULL) *match = *item;
    return true;
}