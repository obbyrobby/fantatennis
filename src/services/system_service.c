#include "../../include/services/system_service.h"
#include "../../include/models/user.h"

bool is_system_empty(const AppContext* app_context) {
    // 1 perchè contiamo l'amministratore di default
    bool users_empty = entity_table_count(&app_context->users_table, NULL, NULL) <= 1;
    bool tournaments_empty = entity_table_count(&app_context->tournaments_table, NULL, NULL) == 0;
    bool matches_empty = entity_table_count(&app_context->matches_table, NULL, NULL) == 0;
    bool teams_empty = entity_table_count(&app_context->teams_table, NULL, NULL) == 0;
    bool players_empty = entity_table_count(&app_context->tennisplayers_table, NULL, NULL) == 0;
    bool participations_empty = entity_table_count(&app_context->participations_table, NULL, NULL) == 0;

    return users_empty && tournaments_empty && matches_empty && teams_empty && players_empty && participations_empty;
}

void clear_system(AppContext* app_context) {
    clear_table(&app_context->users_table);
    clear_table(&app_context->tournaments_table);
    clear_table(&app_context->matches_table);
    clear_table(&app_context->teams_table);
    clear_table(&app_context->tennisplayers_table);
    clear_table(&app_context->participations_table);

    // Se non ci sono utenti, crea l'admin di default
    if (app_context->users_table.count == 0) {
        User default_admin = user_build(DEFAULT_ADMIN_USERNAME, DEFAULT_ADMIN_PASSWORD, true);
        entity_table_add_item(&app_context->users_table, &default_admin);
    }

    app_context->current_user_id = 0;
}
