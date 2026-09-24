#include "../../include/cli/cli_mainmenu.h"
#include "../../include/cli/cli_utils.h"
#include "../../include/services/auth_service.h"
#include "../../include/cli/submodules/cli_tennisplayers.h"
#include "../../include/services/seeding_service.h"
#include "../../include/cli/submodules/cli_leaderboards.h"
#include "../../include/cli/submodules/cli_matches.h"
#include "../../include/cli/submodules/cli_play.h"
#include "../../include/cli/submodules/cli_tournaments.h"
#include <stdio.h>

#include "../../include/services/system_service.h"

static void cli_logout(AppContext* app_context);
static void cli_seed_system(AppContext* app_context, void* data);
static void cli_setadmins(AppContext* app_context, void* data);


void cli_adminmenu(AppContext* app_context) {
    cli_clear_screen();
    printf("\n\n");
    printf("#######################\n");
    printf("#   MENU' PRINCIPALE  #\n");
    printf("#  PER AMMINISTRATORI #\n");
    printf("#######################\n");

    const MenuOption options[] = {
        { "Gestione Tennisti", cli_admin_menu_tennisplayers },
        { "Gestione Tornei", cli_admin_menu_tournaments },
        { "Gestione Partite", cli_admin_menu_matches },
        { "Visualizza Classifica Globale Tennisti", cli_global_tennisplayers_leaderboard },
        { "Visualizza Classifica Globale Utenti", cli_global_users_leaderboard },
        { "Imposta Amministratori", cli_setadmins },
        { "Aggiungi dati di esempio al sistema (Seeding)", cli_seed_system }
    };
    int num_options = sizeof(options) / sizeof(options[0]);
    cli_run_menu(app_context, options, num_options, "Logout", NULL);
    cli_logout(app_context);
}

void cli_regularmenu(AppContext* app_context) {
    cli_clear_screen();
    printf("\n\n");
    printf("######################\n");
    printf("#  MENU' PRINCIPALE  #\n");
    printf("######################\n");

    const MenuOption options[] = {
        { "Visualizza Tennisti", cli_search_menu_tennisplayers },
        { "Visualizza Tornei", cli_view_tournaments },
        { "Visualizza Partite", cli_view_matches },
        { "Visualizza Classifica Globale Tennisti", cli_global_tennisplayers_leaderboard }, // cli_tennisplayers_leaderboard
        { "Visualizza Classifica Globale Utenti", cli_global_users_leaderboard }, // cli_users_leaderboard
        { "Iscriviti ad un Torneo", cli_sign_up_tournament },
        { "Gioca ad un Torneo", cli_play_tournament }
    };
    int num_options = sizeof(options) / sizeof(options[0]);
    cli_run_menu(app_context, options, num_options, "Logout", NULL);
    cli_logout(app_context);
}

static void cli_setadmins(AppContext* app_context, void* data) {
    (void)data;
    size_t users_count;
    User* users = get_users(app_context, true, &users_count);
    if (users_count == SIZE_MAX) {
        printf("\nIl sistema ha terminato la memoria disponibile. Impossibile continuare.\n");
        return;
    }
    printf("\nLista di Utenti\n");
    for (size_t i = 0; i < users_count; i++) {
        printf("[ID %d] %s\n", users[i].id, users[i].username);
    }
    printf("\nInserisci l'ID dell'Utente che desideri rendere un admin, o lascia vuoto per annullare. ");
    int input;
    if (!cli_read_int(1, INT32_MAX, &input, true, false)) {
        free(users);
        return;
    }

    User selected_user;
    if (!get_user_by_id(app_context, (uint32_t)input, &selected_user)) {
        printf("\nE' stato inserito un ID non associato ad alcun Utente.\n");
        free(users);
        return;
    }

    if (selected_user.is_admin) {
        printf("\nQuesto utente è già un admin!\n");
        free(users);
        return;
    }

    printf("\nSei sicuro/a di voler rendere %s un amministratore nel sistema? Questa operazione è irreversibile!\n1) Sì\n2) No\n", selected_user.username);
    cli_read_int(1, 2, &input, false, false);

    if (input == 1) {
        ServiceStatus status = set_admin(app_context, selected_user.id);
        printf("\n%s\n", cli_service_status_to_str(status));
    } else {
        printf("\nOperazione annullata.\n");
    }

    free(users);
}

static void cli_logout(AppContext* app_context) {
    logout(app_context);
    printf("\nLogout eseguito con successo. La sessione è terminata.\n");
}

static void cli_seed_system(AppContext* app_context, void* data) {
    (void)data;
    if (!is_system_empty(app_context)) {
        printf("\nIl sistema non è vuoto. Al fine di mantenere un database di dati coerenti,\nil seeding è "
               "ammesso solo se tutte le tabelle sono vuote, in particolare per via dei riferimenti tra entità con ID come chiavi primarie.\n"
               "E' possibile resettare l'intero sistema prima, per poi procedere col seeding.\n"
               "Desideri azzerare ogni tabella per poi procedere col seeding? Questa opzione non è reversibile.\n1) Sì\n2) No\n");
        int choice;
        cli_read_int(1, 2, &choice, false, false);
        if (choice == 2) {
            printf("\nOperazione annullata.\n");
            return;
        }
        printf("\nSei sicuro/a al 100 percento? Ultima chance! Sarai sloggato dal tuo account al termine dell'operazione.\n1) Sì\n2) No\n");
        cli_read_int(1, 2, &choice, false, false);
        if (choice == 2) {
            printf("\nOperazione annullata.\n");
            return;
        }
    }
    printf("\nSeeding del sistema in corso...\n");
    seed_system(app_context);
    printf("\nSeeding completato.\n");
}

