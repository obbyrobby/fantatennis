#include "../../../include/cli/submodules/cli_play.h"
#include "../../../include/cli/submodules/cli_tennisplayers.h"
#include "../../../include/models/tournament.h"
#include "../../../include/models/team.h"
#include "../../../include/services/tournaments_service.h"
#include "../../../include/services/teams_service.h"
#include "../../../include/cli/cli_utils.h"
#include "../../../include/cli/submodules/cli_tournaments.h"
#include "../../../include/services/players_service.h"
#include "../../../include/services/scoring_service.h"
#include "../../../include/cli/submodules/cli_leaderboards.h"
#include "../../../include/cli/submodules/cli_matches.h"
#include <stdio.h>

static void cli_build_tournament_team(AppContext* app_context, Tournament* tournament, Team* team) {
    bool input_result = true;
    size_t participants_count;
    TennisPlayer* participants = get_tournament_players(app_context, tournament->id, &participants_count);

    if (participants_count == SIZE_MAX) {
        printf("\nIl sistema ha esaurito la memoria disponibile. Impossibile proseguire.\n");
        return;
    }

    if (participants == NULL) {
        printf("\nNon ci sono ancora tennisti partecipanti al torneo!\n");
        return;
    }

    if (participants_count < 6) {
        free(participants);
        printf("\nNon ci sono ancora abbastanza tennisti partecipanti al torneo per comporre una squadra!\n");
        return;
    }

    do {
        unsigned int budget = 0;
        printf("\nTennisti partecipanti al torneo:\n");
        // Non possiamo usare cli_view_tennisplayers qui perchè quella funzione non permette di mostrare il costo in crediti.
        // Non potremmo mostrarlo lì a prescindere perchè il costo è calcolato anche in base al torneo!!
        // Mostriamo qui, e pace
        for (size_t i = 0; i < participants_count; i++) {
            unsigned int cost = get_player_cost(app_context, tournament->id, participants[i].id);
            printf("[ID %d] %s %s, rank %d, costo %d crediti\n",
                participants[i].id, participants[i].name, participants[i].surname, participants[i].ranking, cost);
        }

        printf("\nIl tuo Team:\n");
        for (int i = 0; i < TEAM_SIZE; i++) {
            TennisPlayer curr_player = { 0 };
            if (!get_player_by_id(app_context, team->players_ids[i], &curr_player)) {
                printf("[%d] Non impostato\n", i+1);
            } else {
                unsigned int cost = get_player_cost(app_context, tournament->id, team->players_ids[i]);
                printf("%d) [ID %d] %s %s [Costo: %d]\n", i+1, curr_player.id, curr_player.name, curr_player.surname, cost);
                budget += cost;
            }
        }

        printf("\nCrediti usati: %d/%d", budget, MAX_TEAM_BUDGET);
        if (budget > MAX_TEAM_BUDGET) {
            printf(" (Hai superato il massimo! Questo Team non verrà accettato.)");
        }

        printf("\nInserisci l'ID di un Tennista per aggiungerlo alla squadra se non è già presente,\n"
               "rimuoverlo se è già presente nella squadra, o lascia vuoto per confermare: ");
        int input;
        input_result = cli_read_int(1, INT32_MAX, &input, true, false);

        if (!input_result) {
            break;
        }
        uint32_t id_choice = (uint32_t)input;

        // Una volta dato l'input, verifichiamo se è l'ID di un Tennista già presente nella Squadra o meno
        bool removed = false;

        for (int i = 0; i < TEAM_SIZE; i++) {
            if (team->players_ids[i] == id_choice) {
                // Rimuoviamo questo Tennista
                team->players_ids[i] = 0;
                removed = true;
            }
        }

        // Allora si tratta di un inserimento
        if (!removed) {
            // Verifichiamo prima che sia un Tennista che appartiene al torneo
            bool found = false;

            for (size_t i = 0; i < participants_count; i++) {
                if (participants[i].id == id_choice) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                printf("\nL'ID %d non è associato ad alcun Tennista nel torneo!\n", id_choice);
                continue;
            }

            // Troviamo il primo slot disponibile
            bool inserted = false;
            for (int i = 0; i < TEAM_SIZE; i++) {
                if (team->players_ids[i] == 0) {
                    // Trovato! Inseriamolo qua
                    team->players_ids[i] = id_choice;
                    inserted = true;
                    break;
                }
            }

            if (!inserted) {
                printf("\nIl team è pieno! Non puoi aggiungere ulteriori Tennisti. Puoi rimuoverne alcuni per fare spazio ad altri.\n");
            }
        }
    } while (input_result);
    free(participants);

    ServiceStatus status = set_team(app_context, tournament->id, team->players_ids);
    printf("\n%s\n", cli_service_status_to_str(status));
}

static void cli_view_tournament_team(AppContext* app_context, void* tournament_ptr) {
    Tournament* tournament = tournament_ptr;
    Team player_team = { 0 };

    bool team_found = false;

    // Il team è impostato
    if (get_specific_team(app_context, tournament->id, app_context->current_user_id, &player_team) && is_team_set(&player_team)) {
        team_found = true;
        printf("\nLa tua Squadra:\n");
        for (int i = 0; i < TEAM_SIZE; i++) {
            TennisPlayer player;
            get_player_by_id(app_context, player_team.players_ids[i], &player);
            printf("[Tennista %d] %s %s, rank %d", i+1, player.name, player.surname, player.ranking);
            // Se il torneo è iniziato, mostriamo anche i punti
            if (tournament->state != TOURNAMENT_STATE_OPEN) {
                 printf(" Punti: %.1lf", get_player_tournament_score(app_context, tournament->id, player.id));
            }
            printf("\n");
        }
    }

    if (tournament->state != TOURNAMENT_STATE_OPEN) {
        if (team_found) {
            printf("\n\nIl torneo è iniziato, e di conseguenza la squadra non è più modificabile.\n\n");
        } else {
            printf("\n\nIl Torneo non è più aperto, e di conseguenza non puoi più comporre una squadra e partecipare.\n\n");
        }
        return;
    }

    if (!team_found) {
        printf("\nNon hai ancora composto una squadra per questo Torneo. Vuoi comporla ora? ");
    } else {
        printf("\nVuoi modificare la tua squadra? ");
    }
    printf("\n1) Si'\n2) No\n");
    int choice;
    cli_read_int(1, 2, &choice, false, false);

    if (choice == 1) {
        cli_build_tournament_team(app_context, tournament, &player_team);
    }

}

static void cli_view_tournament_matches(AppContext* app_context, void* tournament_ptr) {
    cli_view_matches(app_context, tournament_ptr);
}

void cli_view_players_leaderboard(AppContext* app_context, void* tournament_ptr) {
    size_t scores_count;
    Tournament* tournament = tournament_ptr;
    PlayerScore* player_scores = get_tournament_players_leaderboard(app_context, tournament->id, &scores_count);

    if (scores_count == SIZE_MAX) {
        printf("\nIl sistema ha esaurito la memoria disponibile. Impossibile continuare.\n");
        return;
    }
    if (player_scores == NULL) {
        printf("\nLa classifica non ha ancora punteggi.\n");
        return;
    }

    cli_print_tennisplayers_leaderboard(app_context, player_scores, scores_count);
    free(player_scores);
}

void cli_view_users_leaderboard(AppContext* app_context, void* tournament_ptr) {
    size_t scores_count;
    Tournament* tournament = tournament_ptr;
    UserScore* user_scores = get_tournament_users_leaderboard(app_context, tournament->id, &scores_count);

    if (scores_count == SIZE_MAX) {
        printf("\nIl sistema ha esaurito la memoria disponibile. Impossibile continuare.\n");
        return;
    }
    if (user_scores == NULL) {
        printf("\nLa classifica non ha ancora punteggi.\n");
        return;
    }

    cli_print_users_leaderboard(app_context, user_scores, scores_count);
    free(user_scores);
}

void cli_sign_up_tournament(AppContext* app_context, void* data) {
    (void)data;
    cli_clear_screen();
    size_t tournaments_count;
    Tournament* open_tournaments = get_tournaments(app_context, FILTER_TOURNAMENT_STATE_OPEN, &tournaments_count);

    if (tournaments_count == SIZE_MAX) {
        printf("\nIl sistema ha esaurito la memoria disponibile. Impossibile proseguire.\n");
        return;
    }

    if (tournaments_count == 0) {
        printf("\nNon ci sono tornei aperti al momento!\n");
        return;
    }

    size_t teams_count;
    Team* user_teams = get_user_teams(app_context, app_context->current_user_id, &teams_count);

    if (teams_count == SIZE_MAX) {
        printf("\nIl sistema ha esaurito la memoria disponibile. Impossibile proseguire.\n");
        if (open_tournaments != NULL) free(open_tournaments);
        return;
    }

    bool output = false;
    for (size_t i = 0; i < tournaments_count; i++) {
        bool already_signed_up = false;
        for (size_t j = 0; j < teams_count; j++) {
            if (user_teams[j].tournament_id == open_tournaments[i].id) {
                already_signed_up = true;
                break;
            }
        }
        if (!already_signed_up) {
            cli_print_single_tournament(app_context, &open_tournaments[i]);
            output = true;
        }
    }

    if (!output) {
        printf("\nNon ci sono tornei aperti al momento ai quali non sei già iscritto/a!\n");
        free(user_teams);
        free(open_tournaments);
        return;
    }

    printf("\nInserisci l'ID del torneo al quale desideri iscriverti (o lascia vuoto per annullare): ");
    int choice;
    if (cli_read_int(1, INT32_MAX, &choice, true, false)) {
        uint32_t requested_tournament_id = (uint32_t)choice;
        ServiceStatus status = sign_up_tournament(app_context, requested_tournament_id);
        printf("\n%s\n", cli_service_status_to_str(status));
    }
    free(user_teams);
    free(open_tournaments);
}

void cli_play_tournament(AppContext* app_context, void* data) {
    (void)data;
    cli_clear_screen();
    // Prima l'utente seleziona un torneo, poi mostriamo il submenu di gioco inerente a quel torneo
    size_t teams_count;
    Team* user_teams = get_user_teams(app_context, app_context->current_user_id, &teams_count);

    if (teams_count == SIZE_MAX) {
        printf("\nIl sistema ha esaurito la memoria disponibile. Impossibile proseguire.\n");
        return;
    }
    if (teams_count == 0) {
        printf("\nNon sei iscritto ad alcun torneo! Puoi iscriverti ad un Torneo in \"Iscriviti ad un Torneo\".");
        return;
    }

    for (size_t i = 0; i < teams_count; i++) {
        Tournament curr_tournament;
        if (!get_tournament_by_id(app_context, user_teams[i].tournament_id, &curr_tournament)) {
            // Questo Team punta ad un Torneo che non esiste... non dovrebbe succedere. Ignoriamo
            continue;
        }
        cli_print_single_tournament(app_context, &curr_tournament);
    }

    printf("\nInserisci l'ID del torneo al quale desideri giocare: ");
    int choice;
    if (!cli_read_int(1, INT32_MAX, &choice, true, false)) {
        printf("\nOperazione annullata.\n");
        return;
    }
    uint32_t requested_tournament_id = (uint32_t)choice;

    Tournament selected_tournament = { 0 };
    for (size_t i = 0; i < teams_count; i++) {
        if (requested_tournament_id == user_teams[i].tournament_id) {
            get_tournament_by_id(app_context, user_teams[i].tournament_id, &selected_tournament);
            break;
        }
    }

    free(user_teams);
    if (selected_tournament.id == 0) {
        printf("\nL'ID inserito non è associato ad alcun torneo a cui sei iscritto/a.\n");
        return;
    }

    // Mostriamo il ranking del Fantacoach attuale
    if (selected_tournament.state != TOURNAMENT_STATE_OPEN) {
        size_t scores_count;
        UserScore* scores = get_tournament_users_leaderboard(app_context, selected_tournament.id, &scores_count);

        if (scores != NULL) {
            for (size_t i = 0; i < scores_count; i++) {
                if (scores[i].user_id == app_context->current_user_id) {
                    printf("\nSei in posizione n° %zu con %.1f punti!\n", i+1, scores[i].score);
                    break;
                }
            }
            free(scores);
        }
    }
    cli_print_single_tournament(app_context, &selected_tournament);

    const MenuOption options[] = {
        {"La mia Squadra", cli_view_tournament_team},
        {"Visualizza Partite", cli_view_tournament_matches},
        {"Visualizza Classifica Tennisti", cli_view_players_leaderboard},
        {"Visualizza Classifica FantaCoach", cli_view_users_leaderboard},
    };
    int num_options = sizeof(options) / sizeof(options[0]);
    cli_run_menu(app_context, options, num_options, "Torna indietro", &selected_tournament);
}
