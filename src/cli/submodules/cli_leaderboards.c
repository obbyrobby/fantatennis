#include "../../../include/cli/submodules/cli_leaderboards.h"
#include "../../../include/services/players_service.h"
#include "../../../include/services/scoring_service.h"
#include "../../../include/services/auth_service.h"
#include "../../../include/models/user.h"
#include <stdio.h>

#include "../../../include/cli/cli_utils.h"

void cli_print_tennisplayers_leaderboard(const AppContext* app_context, const PlayerScore* scores, const size_t scores_count) {
    for (size_t i = 0; i < scores_count; i++) {
        TennisPlayer player;
        if (!get_player_by_id(app_context, scores[i].tennisplayer_id, &player)) {
            continue;
        }
        printf("[%zu] %s %s con %.1lf punti\n", i+1, player.name, player.surname, scores[i].score);
    }
}

void cli_print_users_leaderboard(const AppContext* app_context, const UserScore* scores, const size_t scores_count) {
    for (size_t i = 0; i < scores_count; i++) {
        User user;
        if (!get_user_by_id(app_context, scores[i].user_id, &user)) {
            continue;
        }
        printf("[%zu] %s con %.1lf punti\n", i+1, user.username, scores[i].score);
    }
}

void cli_global_tennisplayers_leaderboard(AppContext* app_context, void* data) {
    (void)data;
    cli_clear_screen();
    size_t scores_count;
    PlayerScore* scores = get_global_players_leaderboard(app_context, &scores_count);
    if (scores_count == SIZE_MAX) {
        printf("\nIl sistema ha terminato la memoria disponibile. Impossibile eseguire questa operazione.\n");
        return;
    }
    if (scores_count <= 0) {
        printf("\nLa classifica è vuota!\n");
        return;
    }

    printf("\nClassifica globale dei Tennisti:\n");
    cli_print_tennisplayers_leaderboard(app_context, scores, scores_count);
    free(scores);
}

void cli_global_users_leaderboard(AppContext* app_context, void* data) {
    cli_clear_screen();
    (void)data;
    size_t scores_count;
    UserScore* scores = get_global_users_leaderboard(app_context, &scores_count);
    if (scores_count == SIZE_MAX) {
        printf("\nIl sistema ha terminato la memoria disponibile. Impossibile eseguire questa operazione.\n");
        return;
    }
    if (scores_count <= 0) {
        printf("\nLa classifica è vuota!\n");
        return;
    }

    printf("\nClassifica globale dei Fanta-Coach:\n");
    cli_print_users_leaderboard(app_context, scores, scores_count);
    free(scores);
}
