#include "../../../include/cli/submodules/cli_tennisplayers.h"
#include "../../../include/models/tennis_player.h"
#include "../../../include/cli/cli_utils.h"
#include "../../../include/services/players_service.h"
#include <stdio.h>
#include <string.h>

static void cli_view_single_tennisplayer(TennisPlayer* player) {
    char availability[16];

    if (player->is_available) {
        strcpy(availability, "Disponibile");
    } else {
        strcpy(availability, "Non Disponibile");
    }

    printf("[ID %d] %s %s, rank %d, %s\n",
        player->id, player->name, player->surname, player->ranking, availability);
}

static void cli_set_tennisplayer(TennisPlayer* player) {
    bool input_result = true;
    do {
        printf("Dati del Tennista:\n1) Nome: %s\n2) Cognome: %s\n3) Disponibilità: %s\n4) Ranking, %d\nInserire l'indice di un campo "
           "per modificarlo, o lascia vuoto e premi Invio per confermare: ",
        strlen(player->name) == 0 ? "[Non impostato]" : player->name,
        strlen(player->surname) == 0 ? "[Non impostato]" : player->surname,
        player->is_available ? "Disponibile" : "Non Disponibile",
        player->ranking);

        int selection;
        input_result = cli_read_int(1, 4, &selection, true, false);

        if (!input_result) break;

        switch (selection) {
            case 1:
                printf("\nInserire il nuovo nome: ");
                cli_read_string(player->name, sizeof(player->name));
                break;
            case 2:
                printf("\nInserire il nuovo cognome: ");
                cli_read_string(player->surname, sizeof(player->surname));
                break;
            case 3:
                printf("\nSelezionare la disponibilità:\n1) Disponibile\n2) Non disponibile\n");
                int availability;
                cli_read_int(1, 2, &availability, false, false);
                player->is_available = availability == 1;
                break;
            case 4:
                printf("\nInserire il nuovo ranking: ");
                cli_read_int(1, MAX_TENNISPLAYERS, &player->ranking, false, false);
                break;
            default:
                break;
        }
    } while (input_result);
}

void cli_view_tennisplayers(TennisPlayer* players, const size_t count, bool compact_view) {
    if (count == 0) {
        printf("\nNon sono presenti tennisti che corrispondono ai criteri di ricerca specificati.\n");
        return;
    }
    if (compact_view) {
        for (size_t i = 0; i < count; i++) {
            printf("[%d] %s %s", players[i].id, players[i].name, players[i].surname);
            // Se ci sono ancora altri giocatori, mettiamo una virgola
            if (i < count - 1) {
                printf(",");
            }

            // Mandiamo a capo ogni 3 giocatori
            if ((i+1) % 3 == 0) {
                printf("\n");
            } else {
                printf("  ");
            }
        }
    } else {
        for (size_t i = 0; i < count; i++) {
            cli_view_single_tennisplayer(&players[i]);
        }
    }

}

static bool cli_select_tennisplayer(AppContext* app_context, TennisPlayer* out_player) {
    size_t players_count;
    TennisPlayer* all_players = get_tennis_players(app_context, NULL, &players_count);
    if (players_count == 0) {
        printf("\nNon sono presenti Tennisti nel sistema!\n");
        return false;
    }
    cli_view_tennisplayers(all_players, players_count, false);
    if (all_players != NULL) {
        free(all_players);
    }

    int input;
    printf("\nInserire l'ID del Tennista interessato, o lasciare vuoto per annullare l'operazione: ");

    if (!cli_read_int(1,INT32_MAX, &input, true, false)) {
        printf("\nOperazione annullata.\n");
        return false;
    }
    uint32_t selected_player_id = (uint32_t)input;
    if (!get_player_by_id(app_context, selected_player_id, out_player)) {
        printf("\nNon esiste un tennista con ID %d.\n", selected_player_id);
        return false;
    }
    return true;
}

static void cli_add_tennisplayer(AppContext* app_context, void* data) {
    (void)data; // Zittisce il compilatore
    TennisPlayer new_player = { 0 };
    cli_set_tennisplayer(&new_player);

    uint32_t newplayer_id;
    ServiceStatus status = add_tennis_player(app_context, &new_player, &newplayer_id);
    printf("\n%s\n", cli_service_status_to_str(status));
}

static void cli_edit_tennisplayer(AppContext* app_context, void* data) {
    (void)data; // Zittisce il compilatore
    TennisPlayer player_to_edit;
    if (!cli_select_tennisplayer(app_context, &player_to_edit)) return;

    cli_set_tennisplayer(&player_to_edit);
    ServiceStatus status = edit_tennis_player(app_context, &player_to_edit);
    printf("\n%s\n", cli_service_status_to_str(status));
}

static void cli_delete_tennisplayer(AppContext* app_context, void* data) {
    (void)data; // Zittisce il compilatore

    TennisPlayer player_to_delete;
    if (!cli_select_tennisplayer(app_context, &player_to_delete)) return;

    printf("\nSei sicuro/a di voler eliminare dal sistema il tennista %s %s? Questa operazione non è reversibile!\n1) Sì\n2) No\n",
        player_to_delete.name, player_to_delete.surname);
    int choice;
    cli_read_int(1, 2, &choice, false, false);
    if (choice == 2) {
        printf("\nOperazione annullata.\n");
        return;
    }
    ServiceStatus status = delete_tennis_player(app_context, player_to_delete.id);
    printf("\n%s\n", cli_service_status_to_str(status));
}

// Lasciare AppContext* app_context non const!!
void cli_search_menu_tennisplayers(AppContext* app_context, void* data) {
    // Zittisce il compilatore
    (void)data;
    cli_clear_screen();
    PlayerSearchFilter filter = { 0 };
    filter.availability_filter = FILTER_NOT_SET;
    bool input_result = true;

    do {
        char availability_filter_str[16];
        switch (filter.availability_filter) {
            case FILTER_TRUE:
                strcpy(availability_filter_str, "Disponibile");
                break;
            case FILTER_FALSE:
                strcpy(availability_filter_str, "Non Disponibile");
                break;
            case FILTER_NOT_SET:
                strcpy(availability_filter_str, "[Non impostato]");
                break;
        }

        printf("Filtri di ricerca\n1) Nome: %s\n2) Cognome: %s\n3) Stato: %s\nInserisci l'indice di un filtro "
               "per impostarlo, o lascia vuoto e premi Invio per avviare la ricerca: ",
               strlen(filter.name_filter) == 0 ? "[Non impostato]" : filter.name_filter,
               strlen(filter.surname_filter) == 0 ? "[Non impostato]" : filter.surname_filter,
               availability_filter_str);
        int selection;
        input_result = cli_read_int(1, 4, &selection, true, false);

        if (!input_result) break;

        switch (selection) {
            case 1:
                printf("\nInserire il nome da cercare: ");
                cli_read_string(filter.name_filter, sizeof(filter.name_filter));
                break;
            case 2:
                printf("\nInserire il cognome da cercare: ");
                cli_read_string(filter.surname_filter, sizeof(filter.surname_filter));
                break;
            case 3:
                printf("\nSelezionare lo stato richiesto per cui filtrare: \n1) Disponibile\n2) Non disponibile\n3) Resetta filtro");
                int availability_int = 3;
                if (!cli_read_int(1, 3, &availability_int, true, true)) {
                    filter.availability_filter = FILTER_NOT_SET;
                } else {
                    filter.availability_filter = availability_int;
                }
                break;
            default:
                // ???
                break;
        }
    } while (input_result);
    size_t players_count;
    TennisPlayer* players = get_tennis_players(app_context, &filter, &players_count);
    if (players_count == SIZE_MAX) {
        printf("\nIl sistema ha terminato la memoria disponibile. Impossibile continuare.\n");
    } else {
        cli_view_tennisplayers(players, players_count, false);
    }

    if (players != NULL) {
        free(players);
    }

}

void cli_admin_menu_tennisplayers(AppContext* app_context, void* data) {
    (void)data; // Zittisce il compilatore
    cli_clear_screen();
    const MenuOption options[] = {
        {"Visualizza Tennisti", cli_search_menu_tennisplayers},
        {"Aggiungi nuovo Tennista", cli_add_tennisplayer},
        {"Modifica Tennista", cli_edit_tennisplayer},
        {"Elimina Tennista", cli_delete_tennisplayer},
    };
    int num_options = sizeof(options) / sizeof(options[0]);
    cli_run_menu(app_context, options, num_options, "Torna indietro", NULL);
}
