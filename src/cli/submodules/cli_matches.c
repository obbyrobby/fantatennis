#include "../../../include/cli/submodules/cli_matches.h"
#include "../../../include/app_context.h"
#include "../../../include/cli/cli_utils.h"
#include "../../../include/cli/submodules/cli_tournaments.h"
#include "../../../include/cli/submodules/cli_tennisplayers.h"
#include "../../../include/services/matches_service.h"
#include "../../../include/services/players_service.h"
#include "../../../include/services/tournaments_service.h"
#include <stdio.h>

static void cli_set_match(const AppContext* app_context, Match* match, const uint32_t tournament_id) {
    size_t players_count;
    TennisPlayer* tournament_players = get_tournament_players(app_context, tournament_id, &players_count);
    if (players_count == SIZE_MAX) {
        printf("\nIl sistema ha esaurito la memoria. Impossibile continuare.\n");
        return;
    }
    if (players_count == 0) {
        printf("\nNon ci sono Tennisti partecipanti al torneo! Impossibile creare una partita.\n");
        return;
    }
    bool input_result = true;
    do {
        PlayerResult* first_result = &match->player_results[0];
        PlayerResult* second_result = &match->player_results[1];

        TennisPlayer first_player, second_player;
        bool first_player_set = get_player_by_id(app_context, first_result->player_id, &first_player);
        bool second_player_set = get_player_by_id(app_context, second_result->player_id, &second_player);

        printf("Dati della partita:\n1) Partecipanti: %s %s contro %s %s\n2) Set vinti: %d a %d\n"
               "3) Ace: %d - %d\n4) Doppi Falli: %d - %d\n5) Break: %d - %d\nInserire l'indice di "
               "un campo per modificarlo, o lascia vuoto e premi invio per confermare: ",
               first_player_set? first_player.name : "",
               first_player_set? first_player.surname : "[Non Impostato]",
               second_player_set? second_player.name : "",
               second_player_set? second_player.surname : "[Non Impostato]",
               first_result->won_sets, second_result->won_sets,
               first_result->aces, second_result->aces,
               first_result->double_faults, second_result->double_faults,
               first_result->breaks, second_result->breaks);

        int selection;
        input_result = cli_read_int(1, 5, &selection, true, false);

        if (!input_result) break;

        switch (selection) {
            case 1:
                printf("\nTennisti partecipanti al torneo:\n");
                int player1_input, player2_input;
                bool player1_found = false, player2_found = false;
                cli_view_tennisplayers(tournament_players, players_count, true);
                printf("\nInserire l'ID del primo tennista che ha partecipato a questa partita, o lasciare vuoto per annullare: ");
                if (!cli_read_int(1, INT32_MAX, &player1_input, true, false)) {
                    break;
                }
                printf("\nInserire l'ID del secondo tennista che ha partecipato a questa partita, o lasciare vuoto per annullare: ");
                if (!cli_read_int(1, INT32_MAX, &player2_input, true, false)) {
                    break;
                }
                for (size_t i = 0; i < players_count; i++) {
                    if (tournament_players[i].id == (uint32_t)player1_input) {
                        player1_found = true;
                    }
                    if (tournament_players[i].id == (uint32_t)player2_input) {
                        player2_found = true;
                    }
                    if (player1_found && player2_found) break;
                }

                if (!player1_found) {
                    printf("\n%d non è associato ad un ID dei partecipanti al torneo.\n", player1_input);
                    break;
                }
                if (!player2_found) {
                    printf("\n%d non è associato ad un ID dei partecipanti al torneo.\n", player2_input);
                    break;
                }
                first_result->player_id = (uint32_t)player1_input;
                second_result->player_id = (uint32_t)player2_input;
                break;
            case 2:
                printf("Inserire i set vinti dal primo Tennista, o lasciare vuoto per annullare: ");
                if (!cli_read_int(0, 100, &first_result->won_sets, true, false)) break;
                printf("Inserire i set vinti dal secondo Tennista, o lasciare vuoto per annullare: ");
                if (!cli_read_int(0, 100, &second_result->won_sets, true, false)) break;
                break;
            case 3:
                printf("Inserire gli ace realizzati dal primo Tennista, o lasciare vuoto per annullare: ");
                if (!cli_read_int(0, 100, &first_result->aces, true, false)) break;
                printf("Inserire gli ace realizzati dal secondo Tennista, o lasciare vuoto per annullare: ");
                if (!cli_read_int(0, 100, &second_result->aces, true, false)) break;
                break;
            case 4:
                printf("Inserire i doppi falli realizzati dal primo Tennista, o lasciare vuoto per annullare: ");
                if (!cli_read_int(0, 100, &first_result->double_faults, true, false)) break;
                printf("Inserire i doppi falli realizzati dal secondo Tennista, o lasciare vuoto per annullare: ");
                if (!cli_read_int(0, 100, &second_result->double_faults, true, false)) break;
                break;
            case 5:
                printf("Inserire i break realizzati dal primo Tennista, o lasciare vuoto per annullare: ");
                if (!cli_read_int(0, 100, &first_result->breaks, true, false)) break;
                printf("Inserire i break realizzati dal secondo Tennista, o lasciare vuoto per annullare: ");
                if (!cli_read_int(0, 100, &second_result->breaks, true, false)) break;
                break;
            default:
                break;
        }
    } while (input_result);
    free(tournament_players);
}
static void cli_print_player_result(const TennisPlayer* player, const PlayerResult* result) {
    printf("%s: %d set vinti, %d Ace, %d Doppi falli, %d Break\n",
        player->surname, result->won_sets, result->aces, result->double_faults, result->breaks);
}

static void cli_print_matches(const AppContext* app_context, const Match* matches, const size_t matches_quantity, const bool compact_view) {
    if (matches_quantity == SIZE_MAX) {
        printf("\nIl sistema ha terminato la memoria disponibile. Impossibile continuare.\n");
        return;
    }
    if (matches_quantity == 0) {
        printf("\nNon ci sono partite registrate per il torneo specificato!\n");
        return;
    }
    for (size_t i = 0; i < matches_quantity; i++) {
        TennisPlayer player_1, player_2;
        get_player_by_id(app_context, matches[i].player_results[0].player_id, &player_1);
        get_player_by_id(app_context, matches[i].player_results[1].player_id, &player_2);
        unsigned int winner_index = match_get_winner_index(&matches[i]);
        printf("\n\n[Partita %d] %s %s vs %s %s\n", matches[i].id, player_1.name, player_1.surname, player_2.name, player_2.surname);
        if (!compact_view) {
            cli_print_player_result(&player_1, &matches[i].player_results[0]);
            cli_print_player_result(&player_2, &matches[i].player_results[1]);
            printf("Vincitore: ");
            if (winner_index == 0) {
                printf("%s %s", player_1.name, player_1.surname);
            } else {
                printf("%s %s", player_2.name, player_2.surname);
            }
        }

    }
    printf("\n");
}

static bool cli_select_match(AppContext* app_context, size_t tournament_id, Match* out_match) {
    size_t matches_quantity;
    Match* matches = get_tournament_matches(app_context, tournament_id, &matches_quantity);

    if (matches_quantity == 0) {
        printf("\nNon ci sono partite registrate per il torneo selezionato!\n");
        return false;
    }
    if (matches_quantity == SIZE_MAX) {
        printf("\nIl sistema ha terminato la memoria disponibile. Impossibile continuare.\n");
        return false;
    }

    cli_print_matches(app_context, matches, matches_quantity, false);
    int input;
    printf("\nInserire l'ID della Partita di interesse, o lasciare vuoto per annullare l'operazione: ");

    if (!cli_read_int(1,INT32_MAX, &input, true, false)) {
        printf("\nOperazione annullata.\n");
        return false;
    }
    uint32_t selected_match_id = (uint32_t)input;
    bool found = false;
    for (size_t i = 0; i < matches_quantity; i++) {
        if (matches[i].id == selected_match_id) {
            found = true;
            *out_match = matches[i];
        }
    }
    free(matches);
    if (!found) {
        printf("\nNon esiste una partita nel torneo selezionato con ID %d.\n", selected_match_id);
        return false;
    }
    return true;
}


static void cli_add_match(AppContext* app_context, void* tournament_obj) {
    Tournament* tournament = tournament_obj;
    Match new_match = { 0 };
    if (tournament->state != TOURNAMENT_STATE_ONGOING) {
        printf("\nAggiungere una partita ad un torneo non in corso non è permesso.\n");
        return;
    }
    new_match.tournament_id = tournament->id;
    cli_set_match(app_context, &new_match, tournament->id);
    ServiceStatus status = add_match(app_context, &new_match);
    printf("\n%s\n", cli_service_status_to_str(status));
}

static void cli_edit_match(AppContext* app_context, void* tournament_obj) {
    Tournament* tournament = tournament_obj;
    Match match_to_edit;
    if (tournament->state != TOURNAMENT_STATE_ONGOING) {
        printf("\nModificare partite in un torneo non in corso non è permesso.\n");
        return;
    }
    if (!cli_select_match(app_context, tournament->id, &match_to_edit)) {
        return;
    }
    cli_set_match(app_context, &match_to_edit, tournament->id);
    ServiceStatus status = edit_match(app_context, &match_to_edit);
    printf("\n%s\n", cli_service_status_to_str(status));

}

static void cli_delete_match(AppContext* app_context, void* tournament_obj) {
    Tournament* tournament = tournament_obj;
    Match match_to_delete;
    if (tournament->state != TOURNAMENT_STATE_ONGOING) {
        printf("\nCancellare partite da un torneo non in corso non è permesso.\n");
        return;
    }
    if (!cli_select_match(app_context, tournament->id, &match_to_delete)) {
        return;
    }
    printf("\nSei sicuro/a di voler eliminare dal sistema la partita"
           " con ID %d? Questa operazione non è reversibile!\n1) Sì\n2) No\n", match_to_delete.id);
    int choice;
    cli_read_int(1, 2, &choice, false, false);
    if (choice == 2) {
        printf("\nOperazione annullata.\n");
        return;
    }
    ServiceStatus status = delete_match(app_context, match_to_delete.id);
    printf("\n%s\n", cli_service_status_to_str(status));
}

void cli_view_matches(AppContext* app_context, void* data) {
    cli_clear_screen();
    Tournament tournament;
    if (data == NULL) {
        bool is_selected = cli_select_tournament(app_context, &tournament, FILTER_TOURNAMENT_STATE_ALL);
        if (!is_selected) return;
    } else {
        tournament = *(Tournament*)data;
    }
    size_t matches_quantity;
    Match* matches = get_tournament_matches(app_context, tournament.id, &matches_quantity);

    if (matches_quantity > 0) {
        printf("\nPartite del Torneo %s\n", tournament.title);
        cli_print_matches(app_context, matches, matches_quantity, false);
    } else {
        printf("\nNon ci sono partite registrate per il torneo specificato!\n");
    }
    if (matches != NULL) free(matches);
}

void cli_admin_menu_matches(AppContext* app_context, void* data) {
    (void)data;
    cli_clear_screen();
    Tournament tournament;
    printf("\nPer procedere, selezionare il torneo di cui si desidera gestire le partite.\n");
    bool is_selected = cli_select_tournament(app_context, &tournament, FILTER_TOURNAMENT_STATE_ALL);
    if (!is_selected) return;
    const MenuOption options[] = {
        {"Visualizza Partite", cli_view_matches},
        {"Aggiungi nuova Partita", cli_add_match},
        {"Modifica Partita", cli_edit_match},
        {"Elimina Partita", cli_delete_match},
    };
    int num_options = sizeof(options) / sizeof(options[0]);
    cli_run_menu(app_context, options, num_options, "Torna indietro", &tournament);
}