#include "../include/app_context.h"

#include <stdio.h>

#include "../include/config.h"
#include "../include/models/user.h"
#include "../include/models/tournament.h"
#include "../include/models/match.h"
#include "../include/models/tennis_player.h"
#include "../include/models/team.h"

ReadFileResult init_app_context(AppContext* app_context, char* bad_file) {
    // Iniializziamo le EntityTables

    // Utenti
    ReadFileResult users_result = init_entity_table(&app_context->users_table, sizeof(User), MAX_USERS, USERS_MAGICNUMBER, USERS_FILENAME);
    if (users_result != READFILE_SUCCESS) {
        snprintf(bad_file, 50, "%s", USERS_FILENAME);
        destroy_app_context(app_context);
        return users_result;
    }

    // Tennisti
    ReadFileResult players_result = init_entity_table(&app_context->tennisplayers_table, sizeof(TennisPlayer), MAX_TENNISPLAYERS, TENNISPLAYERS_MAGICNUMBER, TENNISPLAYERS_FILENAME);
    if (players_result != READFILE_SUCCESS) {
        snprintf(bad_file, 50, "%s", TENNISPLAYERS_FILENAME);
        destroy_app_context(app_context);
        return players_result;
    }

    // Tornei
    ReadFileResult tournaments_result = init_entity_table(&app_context->tournaments_table, sizeof(Tournament), MAX_TOURNAMENTS, TOURNAMENTS_MAGICNUMBER, TOURNAMENTS_FILENAME);
    if (tournaments_result != READFILE_SUCCESS) {
        snprintf(bad_file, 50, "%s", TOURNAMENTS_FILENAME);
        destroy_app_context(app_context);
        return tournaments_result;
    }

    // Partite
    ReadFileResult matches_result = init_entity_table(&app_context->matches_table, sizeof(Match), MAX_MATCHES, MATCHES_MAGICNUMBER, MATCHES_FILENAME);
    if (matches_result != READFILE_SUCCESS) {
        snprintf(bad_file, 50, "%s", MATCHES_FILENAME);
        destroy_app_context(app_context);
        return matches_result;
    }

    // Squadre
    ReadFileResult teams_result = init_entity_table(&app_context->teams_table, sizeof(Team), MAX_TEAMS, TEAMS_MAGICNUMBER, TEAMS_FILENAME);
    if (teams_result != READFILE_SUCCESS) {
        snprintf(bad_file, 50, "%s", TEAMS_FILENAME);
        destroy_app_context(app_context);
        return teams_result;
    }

    // Partecipazioni di tennisti a tornei
    ReadFileResult participations_result = init_entity_table(&app_context->participations_table, sizeof(TournamentParticipation), MAX_PARTICIPATIONS, PARTICIPATIONS_MAGICNUMBER, PARTICIPATIONS_FILENAME);
    if (participations_result != READFILE_SUCCESS) {
        snprintf(bad_file, 50, "%s", PARTICIPATIONS_FILENAME);
        destroy_app_context(app_context);
        return participations_result;
    }

    // Se non ci sono utenti, crea l'admin di default
    if (app_context->users_table.count == 0) {
        User default_admin = user_build(DEFAULT_ADMIN_USERNAME, DEFAULT_ADMIN_PASSWORD, true);
        entity_table_add_item(&app_context->users_table, &default_admin);
    }

    app_context->current_user_id = 0; // 0 significa nessun utente. Gli ID inizieranno da 1
    return READFILE_SUCCESS;
}

void destroy_app_context(AppContext* app_context) {
    if (app_context->users_table.data != NULL) free(app_context->users_table.data);
    if (app_context->matches_table.data != NULL) free(app_context->matches_table.data);
    if (app_context->tennisplayers_table.data != NULL) free(app_context->tennisplayers_table.data);
    if (app_context->tournaments_table.data != NULL) free(app_context->tournaments_table.data);
    if (app_context->teams_table.data != NULL) free(app_context->teams_table.data);
    if (app_context->participations_table.data != NULL) free(app_context->participations_table.data);
}
