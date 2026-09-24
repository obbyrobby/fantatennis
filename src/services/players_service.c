#include "../../include/services/players_service.h"
#include "../../include/services/tournaments_service.h"
#include "../../include/utils/string_utils.h"
#include <string.h>


static bool filter_users(const void* element, const void* ctx) {
    const TennisPlayer* player = element;
    const PlayerSearchFilter* filter = ctx;

    if (filter != NULL) {
        // Controlliamo tutti i filtri, uno per uno, per ogni giocatore
        // Prima se sono abilitati, poi se il giocatore rientra in tali filtri

        // Filtro del nome
        if (filter->name_filter[0] != '\0' && !str_contains(player->name, filter->name_filter)) {
            return false;
        }

        // Filtro del cognome
        if (filter->surname_filter[0] != '\0' && !str_contains(player->surname, filter->surname_filter)) {
            return false;
        }

        // Filtro della disponibilità
        if (filter->availability_filter != FILTER_NOT_SET) {
            if (filter->availability_filter == FILTER_FALSE && player->is_available) {
                return false;
            }

            if (filter->availability_filter == FILTER_TRUE && !player->is_available) {
                return false;
            }
        }
    }
    return true;
}

TennisPlayer* get_tennis_players(const AppContext* app_context, const PlayerSearchFilter* filter, size_t* out_count) {
    return entity_table_select(&app_context->tennisplayers_table, filter_users, filter, out_count);
}

unsigned int get_player_cost(const AppContext* app_context, const uint32_t tournament_id, const uint32_t tennisplayer_id) {
    size_t players_count;
    TennisPlayer* tournament_players = get_tournament_players(app_context, tournament_id, &players_count);
    if (tournament_players == NULL) {
        return 0;
    }

    int target_rank = 0; // Il rank del giocatore che ci è stato passato nei parametri
    for (size_t i = 0; i < players_count; i++) {
        if (tournament_players[i].id == tennisplayer_id) {
            target_rank = tournament_players[i].ranking;
            break;
        }
    }

    if (target_rank == 0) {
        // L'ID del Tennista passato non è stato trovato nel Torneo
        free(tournament_players);
        return 0;
    }

    if (players_count <= 1) {
        free(tournament_players);
        return (MAX_PLAYER_COST + MIN_PLAYER_COST) / 2;
    }

    int position = 0;
    for (size_t i = 0; i < players_count; i++) {
        if (tournament_players[i].ranking < target_rank) {
            position++;
        }
    }
    free(tournament_players);

    int offset = position * (MAX_PLAYER_COST - MIN_PLAYER_COST);
    unsigned int cost = MAX_PLAYER_COST - (int)(offset / (players_count - 1));

    if (cost < MIN_PLAYER_COST) cost = MIN_PLAYER_COST;
    if (cost > MAX_PLAYER_COST) cost = MAX_PLAYER_COST;

    return cost;
}

ServiceStatus add_tennis_player(AppContext* app_context, TennisPlayer* new_player, uint32_t* out_id) {
    if (!app_context->is_admin) {
        return STATUS_ERR_UNAUTHORIZED;
    }

    if (new_player->name[0] == '\0') {
        return STATUS_ERR_NAME_EMPTY;
    }

    if (new_player->surname[0] == '\0') {
        return STATUS_ERR_SURNAME_EMPTY;
    }

    if (new_player->ranking == 0) {
        return STATUS_ERR_RANKING_NOT_SET;
    }

    size_t newplayer_id = entity_table_add_item(&app_context->tennisplayers_table, new_player);
    if (newplayer_id == 0) {
        return STATUS_ERR_OOM;
    }

    if (out_id != NULL) {
        (*out_id) = newplayer_id;
    }

    return STATUS_OK;
}

ServiceStatus edit_tennis_player(AppContext* app_context, TennisPlayer* edited_player) {
    if (!app_context->is_admin) {
        return STATUS_ERR_UNAUTHORIZED;
    }

    if (edited_player->name[0] == '\0') {
        return STATUS_ERR_NAME_EMPTY;
    }

    if (edited_player->surname[0] == '\0') {
        return STATUS_ERR_SURNAME_EMPTY;
    }

    if (edited_player->ranking == 0) {
        return STATUS_ERR_RANKING_NOT_SET;
    }

    // Non possiamo modificare giocatori se sono impegnati in tornei attualmente in corso - verifichiamo
    size_t participations = get_player_ongoing_tournaments_quantity(app_context, edited_player->id);

    if (participations > 0) {
        return STATUS_ERR_PLAYER_BUSY;
    }

    if (!entity_table_edit_item(&app_context->tennisplayers_table, edited_player->id, edited_player)) {
        return STATUS_ERR_NOT_FOUND;
    }

    return STATUS_OK;
}

ServiceStatus delete_tennis_player(AppContext* app_context, const uint32_t id) {
    if (!app_context->is_admin) {
        return STATUS_ERR_UNAUTHORIZED;
    }

    size_t participations = get_player_tournaments_quantity(app_context, id);

    if (participations > 0) {
        return STATUS_ERR_PLAYER_HAS_HISTORY;
    }

    if (!entity_table_delete_item(&app_context->tennisplayers_table, id)) {
        return STATUS_ERR_NOT_FOUND;
    }

    return STATUS_OK;
}

bool get_player_by_id(const AppContext* app_context, const uint32_t id, TennisPlayer* player) {
    TennisPlayer* item = entity_table_get_item_by_id(&app_context->tennisplayers_table, id);
    if (item == NULL) return false;
    if (player != NULL) *player = *item;
    return true;
}
