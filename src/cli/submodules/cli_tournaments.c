#include "../../../include/cli/submodules/cli_tournaments.h"
#include "../../../include/services/tournaments_service.h"
#include "../../../include/cli/cli_utils.h"
#include "../../../include/cli/submodules/cli_tennisplayers.h"
#include "../../../include/cli/submodules/cli_play.h"
#include "../../../include/services/players_service.h"
#include "../../../include/common_types.h"
#include "../../../include/models/user.h"
#include "../../../include/services/auth_service.h"
#include "../../../include/services/teams_service.h"
#include <stdio.h>
#include <string.h>


static void cli_set_tournament(AppContext* app_context, Tournament* tournament) {
    bool input_result = true;
    do {
        printf("Dati del Torneo:\n1) Titolo: %s\n2) Stato: %s\nInserire l'indice di un campo "
           "per modificarlo, o lascia vuoto e premi Invio per confermare: ",
            strlen(tournament->title) == 0 ? "[Non impostato]" : tournament->title,
            cli_tournament_state_to_str(tournament->state));

        if (tournament->state == TOURNAMENT_STATE_COMPLETED) {
            TennisPlayer winner;
            get_player_by_id(app_context, tournament->winner_id, &winner);
            printf("3) Vincitore: %s %s\n", winner.surname, winner.name);
        }
        int selection;
        input_result = cli_read_int(1, 2, &selection, true, false);

        if (!input_result) break;

        switch (selection) {
            case 1:
                printf("\nInserire il nuovo Titolo del torneo: ");
                cli_read_string(tournament->title, sizeof(tournament->title));
                break;
            case 2:
                printf("\nInserire il nuovo stato del torneo (o lasciare vuoto per annullare):"
                       "\n1) Aperto\n2) In Corso\n3) Completato\n");
                int choice;
                TennisPlayer* tournament_players;
                size_t players_count;
                if (!cli_read_int(1, 3, &choice, true, false)) {
                    break;
                }
                switch (choice) {
                    case 1:
                        tournament->state = TOURNAMENT_STATE_OPEN;
                        break;
                    case 2:
                        tournament->state = TOURNAMENT_STATE_ONGOING;
                        break;
                    case 3:
                        tournament_players = get_tournament_players(app_context, tournament->id, &players_count);
                        if (players_count == 0) {
                            printf("\nIl torneo non può essere completato se non ha neanche un partecipante!\n");
                            break;
                        }
                        if (players_count == SIZE_MAX) {
                            printf("\nIl sistema ha terminato la memoria disponbiile. Impossibile continuare.\n");
                            break;
                        }

                        tournament->state = TOURNAMENT_STATE_COMPLETED;
                        printf("\nDato che il torneo è terminato, specifica anche un vincitore\n");

                        cli_view_tennisplayers(tournament_players, players_count, false);
                        free(tournament_players);
                        printf("Inserisci l'ID del vincitore: ");
                        int winner_id;
                        cli_read_int(1, INT32_MAX, &winner_id, false, false);

                        tournament->winner_id = winner_id;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    } while (input_result);
}

void cli_print_single_tournament(const AppContext* app_context, Tournament* tournament) {
    size_t num_players = get_tournament_players_quantity(app_context, tournament->id);
    size_t num_teams = get_tournament_teams_quantity(app_context, tournament->id);
    printf("[ID %d] Torneo %s, %zu tennisti partecipanti, %zu Fanta-Coach registrati, %s", tournament->id, tournament->title, num_players, num_teams, cli_tournament_state_to_str(tournament->state));
    if (tournament->state == TOURNAMENT_STATE_COMPLETED) {
        TennisPlayer winner;
        if (get_player_by_id(app_context, tournament->winner_id, &winner)) {
            printf("   Vincitore: %s %s\n", winner.name, winner.surname);
        } else {
            printf("\n");
        }

    } else {
        printf("\n");
    }
}

static void cli_print_tournaments(const AppContext* app_context, Tournament* tournaments, size_t tournaments_count) {
    for (size_t i = 0; i < tournaments_count; i++) {
        cli_print_single_tournament(app_context, &tournaments[i]);
    }
}

// Lasciare app_context non const!!!
void cli_view_tournaments(AppContext* app_context, void* data) {
    (void)data;
    size_t tournaments_count = 0;
    Tournament* tournaments = get_tournaments(app_context, FILTER_TOURNAMENT_STATE_ALL, &tournaments_count);

    if (tournaments_count == 0) {
        printf("\nNon ci sono tornei presenti nel sistema!\n");
        return;
    }

    if (tournaments_count == SIZE_MAX) {
        printf("\nIl sistema ha terminato la memoria disponibile. Impossibile continuare.\n");
        return;
    }

    cli_print_tournaments(app_context, tournaments, tournaments_count);

    if (tournaments != NULL) {
        free(tournaments);
    }
}

bool cli_select_tournament(const AppContext* app_context, Tournament* tournament, FilterTournamentState state_filter) {
    size_t tournaments_count = 0;
    Tournament* tournaments = get_tournaments(app_context, state_filter, &tournaments_count);

    if (tournaments_count == SIZE_MAX) {
        printf("\nIl sistema ha terminato la memoria. Impossibile eseguire l'operazione.\n");
        return false;
    }

    if (tournaments_count == 0) {
        printf("\nNon ci sono tornei presenti nel sistema!\n");
        return false;
    }

    cli_print_tournaments(app_context, tournaments, tournaments_count);
    free(tournaments);

    printf("\nInserisci l'ID del Torneo di interesse (o lascia vuoto per annullare): ");
    int choice;
    if (!cli_read_int(1, INT32_MAX, &choice, true, false)) {
        printf("\nOperazione annullata.\n");
        return false;
    }
    if (!get_tournament_by_id(app_context, choice, tournament)) {
        printf("\nL'ID inserito non appartiene ad alcun Torneo nel sistema.\n");
        return false;
    }
    return true;
}

static void cli_add_tournament(AppContext* app_context, void* data) {
    (void)data;
    Tournament new_tournament = { 0 };
    printf("\nInserire il Titolo del torneo (o lasciare vuoto per annullare): ");
    if (!cli_read_string(new_tournament.title, sizeof(new_tournament.title))) {
        return;
    }
    uint32_t newtournament_id;
    ServiceStatus status = add_tournament(app_context, &new_tournament, &newtournament_id);
    printf("\n%s\n", cli_service_status_to_str(status));
}

static void cli_edit_tournament(AppContext* app_context, void* data) {
    (void)data;
    Tournament tournament_to_edit;
    if (!cli_select_tournament(app_context, &tournament_to_edit, FILTER_TOURNAMENT_STATE_ALL)) return;

    cli_set_tournament(app_context, &tournament_to_edit);
    ServiceStatus status = edit_tournament(app_context, &tournament_to_edit);
    printf("\n%s\n", cli_service_status_to_str(status));
}

static void cli_delete_tournament(AppContext* app_context, void* data) {
    (void)data;
    Tournament tournament;
    if (!cli_select_tournament(app_context, &tournament, FILTER_TOURNAMENT_STATE_ALL)) return;

    printf("\nSei sicuro/a di voler eliminare del sistema il torneo %s? Questa operazione non è reversibile!\n"
           "Saranno eliminate anche tutte le partite, punteggi e squadre inerenti!\n1) Sì\n2) No\n", tournament.title);
    int choice;
    cli_read_int(1, 2, &choice, false, false);
    if (choice == 2) {
        printf("\nOperazione annullata.\n");
        return;
    }

    printf("\nSei davvero davvero sicuro/a? Ultima chance!\n1) Sì\n2) No\n");
    cli_read_int(1, 2, &choice, false, false);
    if (choice == 2) {
        printf("\nOperazione annullata.\n");
        return;
    }

    ServiceStatus status = delete_tournament(app_context, tournament.id);
    printf("\n%s\n", cli_service_status_to_str(status));
}

static void cli_manage_tournament_players(AppContext* app_context, void* data) {
    (void)data;
    Tournament tournament;
    printf("\nSaranno mostrati solo tornei Aperti perchè non è ammesso aggiungere Tennisti "
           "a tornei già iniziati.\n");
    if (!cli_select_tournament(app_context, &tournament, FILTER_TOURNAMENT_STATE_OPEN)) return;

    bool input_result = true;
    do {
        size_t players_count;
        TennisPlayer* players = get_players_not_in_tournament(app_context, tournament.id, &players_count);
        if (players_count == SIZE_MAX) {
            printf("\nIl sistema ha esaurito la memoria disponibile. Impossibile proseguire.\n");
            return;
        }

        size_t participants_count;
        TennisPlayer* participants = get_tournament_players(app_context, tournament.id, &participants_count);
        if (participants_count == SIZE_MAX) {
            printf("\nIl sistema ha esaurito la memoria disponibile. Impossibile proseguire.\n");
            if (players != NULL) free(players);
            return;
        }

        if (participants_count + players_count == 0) {
            printf("\nNon sono presenti Tennisti nel sistema!\n");
            return;
        }

        printf("\nTennisti disponibili:\n");
        cli_view_tennisplayers(players, players_count, false);

        printf("\nTennisti attualmente partecipanti al torneo:\n");
        if (participants_count == 0) {
            printf("Ancora nessuno.\n");
        } else {
            cli_view_tennisplayers(participants, participants_count, true);
        }

        printf("\nInserisci l'ID di un tennista disponibile per aggiungerlo al torneo,\n"
               "l'ID di un tennista già partecipante per rimuoverlo dal torneo, o vuoto per chiudere il menu': ");

        int choice_int;
        input_result = cli_read_int(1, INT32_MAX, &choice_int, true, false);
        if (!input_result) break;
        uint32_t choice = (uint32_t)choice_int;

        // Controlliamo se l'ID appartiene ad un tennista già nel roster; se sì, sappiamo che l'utente vuole togliere,
        // altrimenti, è un inserimento
        bool is_removal = false;
        for (size_t i = 0; i < participants_count; i++) {
            if (choice == participants[i].id) {
                is_removal = true;
                break;
            }
        }

        ServiceStatus status;
        if (is_removal) {
            status = delete_tournament_participation(app_context, choice, tournament.id);
        } else {
            status = add_tournament_participation(app_context, choice, tournament.id);
        }
        printf("\n%s\n", cli_service_status_to_str(status));

        free(players);
        free(participants);
    } while (input_result);
}

static void cli_admin_view_players_leaderboard(AppContext* app_context, void* data) {
    (void)data;
    Tournament tournament;
    if (cli_select_tournament(app_context, &tournament, FILTER_TOURNAMENT_STATE_ALL)) cli_view_players_leaderboard(app_context, &tournament);
}

static void cli_admin_view_users_leaderboard(AppContext* app_context, void* data) {
    (void)data;
    Tournament tournament;
    if (cli_select_tournament(app_context, &tournament, FILTER_TOURNAMENT_STATE_ALL)) cli_view_users_leaderboard(app_context, &tournament);
}

static void cli_view_teams(AppContext* app_context, void* data) {
    (void)data;
    cli_clear_screen();
    Tournament tournament;
    if (!cli_select_tournament(app_context, &tournament, FILTER_TOURNAMENT_STATE_ALL)) return;

    size_t teams_count;
    Team* tournament_teams = get_tournament_teams(app_context, tournament.id, &teams_count);

    if (teams_count == SIZE_MAX) {
        printf("\nIl sistema ha terminato la memoria disponibile. Impossibile continuare.\n");
        return;
    }
    if (tournament_teams == NULL) {
        printf("\nNon ci sono squadre o registrazioni da mostrare!\n");
        return;
    }

    for (size_t i = 0; i < teams_count; i++) {
        User team_owner;
        if (!get_user_by_id(app_context, tournament_teams[i].user_id, &team_owner)) {
            continue;
        }
        printf("%zu) Squadra di %s:\n", i+1, team_owner.username);
        if (is_team_set(&tournament_teams[i])) {
            for (int j = 0; j < TEAM_SIZE; j++) {
                TennisPlayer curr_player;
                get_player_by_id(app_context, tournament_teams[i].players_ids[j], &curr_player);
                printf("    - %s %s\n", curr_player.name, curr_player.surname);
            }
        } else {
            printf("\nNon impostata.\n");
        }
        printf("\n");
    }
    free(tournament_teams);
}

void cli_admin_menu_tournaments(AppContext* app_context, void* data) {
    (void)data;
    cli_clear_screen();
    const MenuOption options[] = {
        {"Visualizza Tornei", cli_view_tournaments},
        {"Visualizza classifica Tennisti di un Torneo", cli_admin_view_players_leaderboard},
        {"Visualizza classifica FantaCoach di un Torneo", cli_admin_view_users_leaderboard},
        {"Visualizza Squadre di un Torneo", cli_view_teams},
        {"Aggiungi nuovo Torneo", cli_add_tournament},
        {"Modifica Torneo", cli_edit_tournament},
        {"Gestisci Roster di un Torneo", cli_manage_tournament_players},
        {"Elimina Torneo", cli_delete_tournament},
    };
    int num_options = sizeof(options) / sizeof(options[0]);
    cli_run_menu(app_context, options, num_options, "Torna indietro", NULL);
}
