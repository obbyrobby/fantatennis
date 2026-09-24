#include "../../include/services/scoring_service.h"
#include "../../include/services/matches_service.h"
#include "../../include/models/team.h"
#include "../../include/services/teams_service.h"
#include "../../include/services/tournaments_service.h"
#include <stdlib.h>

#include "../../include/models/user.h"

static int player_score_comp(const void* item_one, const void* item_two) {
    const PlayerScore* score_one = item_one;
    const PlayerScore* score_two = item_two;

    if (score_one->score == score_two->score) {
        // Se i punteggi sono pari, allora vince il giocatore con l'ID più basso
        if (score_one->tennisplayer_id < score_two->tennisplayer_id) {
            return -1;
        }
        return 1;
    }

    if (score_one->score > score_two->score) {
        return -1;
    }
    return 1;
}

static int user_score_comp(const void* item_one, const void* item_two) {
    const UserScore* score_one = item_one;
    const UserScore* score_two = item_two;

    if (score_one->score == score_two->score) {
        // Se i punteggi sono pari, allora vince il giocatore con l'ID più basso
        if (score_one->user_id < score_two->user_id) {
            return -1;
        }
        return 1;
    }

    if (score_one->score > score_two->score) {
        return -1;
    }
    return 1;
}

double get_match_score(const Match* match, const unsigned int player_index) {
    if (player_index != 0 && player_index != 1) return 0;

    double player_score = 0;
    PlayerResult result = match->player_results[player_index];

    if (match_get_winner_index(match) == (int)player_index) {
        // Vittoria del Match
        player_score += 10;

        // Vittoria in due set
        if (result.won_sets == 2) {
            player_score += 3;
        }
    } else {
        // Sconfitta del match
        player_score -= 3;
    }

    // Bonus Ace
    if (result.aces >= 5) {
        player_score += 2;
    }

    // Bonus Break
    player_score += result.breaks * 0.5;

    // Malus Doppio Fallo
    player_score -= result.double_faults;

    return player_score;
}

double get_player_tournament_score(const AppContext* app_context, const uint32_t tournament_id, const uint32_t tennisplayer_id) {
    size_t matches_count;
    Match* matches = get_tournament_matches(app_context, tournament_id, &matches_count);
    double score = 0;
    if (matches == NULL) return 0;

    for (size_t i = 0; i < matches_count; i++) {
        if (matches[i].player_results[0].player_id == tennisplayer_id) {
            score += get_match_score(&matches[i], 0);
        }
        if (matches[i].player_results[1].player_id == tennisplayer_id) {
            score += get_match_score(&matches[i], 1);
        }
    }
    free(matches);
    return score;
}

double get_user_tournament_score(const AppContext* app_context, uint32_t tournament_id, uint32_t user_id) {
    Team user_team;
    if (!get_specific_team(app_context, tournament_id, user_id, &user_team)) {
        // Team non trovato
        return 0;
    }
    Tournament tournament;
    if (!get_tournament_by_id(app_context, tournament_id, &tournament)) {
        return 0;
    }
    double score = 0;
    for (int i = 0; i < TEAM_SIZE; i++) {
        // Calcoliamo il totale dei punteggi dei giocatori del team
        score += get_player_tournament_score(app_context, tournament_id, user_team.players_ids[i]);

        // Bonus di 10 punti ai Fantacoach che hanno il vincitore del torneo nel loro roster
        if (tournament.state == TOURNAMENT_STATE_COMPLETED && tournament.winner_id == user_team.players_ids[i]) {
            score += 10;
        }
    }

    return score;
}

double get_player_global_score(const AppContext* app_context, const uint32_t tennisplayer_id) {
    size_t tournaments_count;
    double score = 0;
    Tournament* player_tournaments = get_player_tournaments(app_context, tennisplayer_id, &tournaments_count);
    if (tournaments_count == SIZE_MAX) return 0;
    for (size_t i = 0; i < tournaments_count; i++) {
        score += get_player_tournament_score(app_context, player_tournaments[i].id, tennisplayer_id);
    }
    free(player_tournaments);
    return score;
}

double get_user_global_score(const AppContext* app_context, const uint32_t user_id) {
    size_t teams_count;
    Team* user_teams = get_user_teams(app_context, user_id, &teams_count);
    if (teams_count == SIZE_MAX) return 0;
    double score = 0;

    // Usiamo le Squadre dell'utente per sapere su che Tornei iterare
    for (size_t i = 0; i < teams_count; i++) {
        score += get_user_tournament_score(app_context, user_teams[i].tournament_id, user_id);
    }

    free(user_teams);
    return score;
}

PlayerScore* get_tournament_players_leaderboard(const AppContext* app_context, const uint32_t tournament_id, size_t* out_count) {
    TennisPlayer* players = get_tournament_players(app_context, tournament_id, out_count);
    if (players == NULL || *out_count <= 0) return NULL;

    PlayerScore* scores = malloc(sizeof(PlayerScore) * *out_count);
    if (scores == NULL) {
        free(players);
        *out_count = SIZE_MAX;
        return NULL;
    }

    for (size_t i = 0; i < *out_count; i++) {
        scores[i].tennisplayer_id = players[i].id;
        scores[i].score = get_player_tournament_score(app_context, tournament_id, players[i].id);
    }

    qsort(scores, *out_count, sizeof(PlayerScore), player_score_comp);

    free(players);
    return scores;
}

UserScore* get_tournament_users_leaderboard(const AppContext* app_context, const uint32_t tournament_id, size_t* out_count) {
    // Mostriamo anche i FantaCoach con 0 punti, che non hanno impostato un team
    Team* teams = get_tournament_teams(app_context, tournament_id, out_count);
    if (teams == NULL || *out_count <= 0) return NULL;

    UserScore* user_scores = malloc(sizeof(UserScore) * *out_count);
    if (user_scores == NULL) {
        // Non abbiamo memoria per i punteggi dei Fantacoach
        free(teams);
        *out_count = SIZE_MAX;
        return NULL;
    }

    for (size_t i = 0; i < *out_count; i++) {
        double this_user_score = get_user_tournament_score(app_context, tournament_id, teams[i].user_id);
        user_scores[i].user_id = teams[i].user_id;
        user_scores[i].score = this_user_score;
    }

    qsort(user_scores, *out_count, sizeof(UserScore), user_score_comp);
    free(teams);

    return user_scores;
}

PlayerScore* get_global_players_leaderboard(const AppContext* app_context, size_t* out_count) {
    // Mostriamo tutti i Tennisti, anche quelli con 0 punti.
    *out_count = app_context->tennisplayers_table.count;

    if (*out_count == 0) {
        // Non ci sono tennisti nel sistema!
        return NULL;
    }
    PlayerScore* player_scores = malloc(*out_count * sizeof(PlayerScore));
    if (player_scores == NULL) {
        *out_count = SIZE_MAX;
        return NULL;
    }

    TennisPlayer* players = app_context->tennisplayers_table.data;
    for (size_t i = 0; i < app_context->tennisplayers_table.count; i++) {
        player_scores[i].tennisplayer_id = players[i].id;
        player_scores[i].score = get_player_global_score(app_context, players[i].id);
    }

    qsort(player_scores, *out_count, sizeof(PlayerScore), player_score_comp);
    return player_scores;
}

UserScore* get_global_users_leaderboard(const AppContext* app_context, size_t* out_count) {
    // Al contrario delle classifiche specifiche ai tornei, evitiamo di mostrare utenti con 0 punti globalmente,
    // non ha senso. Andremmo praticamente a mostrare ogni utente nel sistema

    // Tuttavia, non possiamo sapere in anticipo quanti utenti andremo a mostrare. Quindi per ora allochiamo il massimo possibile,
    // ovvero abbastanza spazio da contenere ogni utente nel sistema, poi ridimensioniamo dopo

    if (app_context->users_table.count <= 0) {
        // Non ci sono utenti nel sistema!
        return NULL;
    }

    UserScore* user_scores = malloc(app_context->users_table.count * sizeof(UserScore));
    if (user_scores == NULL) {
        *out_count = SIZE_MAX;
        return NULL;
    }

    User* users = app_context->users_table.data;
    *out_count = 0;

    for (size_t i = 0; i < app_context->users_table.count; i++) {
        double user_score = get_user_global_score(app_context, users[i].id);
        if (user_score > 0) {
            user_scores[*out_count].user_id = users[i].id;
            user_scores[*out_count].score = user_score;
            (*out_count)++;
        }
    }

    if (*out_count == 0) {
        // Ci sono utenti nel sistema, ma nessuno che abbia mai fatto punti in qualsiasi torneo
        free(user_scores);
        return NULL;
    }

    qsort(user_scores, *out_count, sizeof(UserScore), user_score_comp);

    UserScore* resized_scores = realloc(user_scores, sizeof(UserScore) * *out_count);
    if (resized_scores == NULL) {
        return user_scores;
    }
    return resized_scores;
}
