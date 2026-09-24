#include "../../include/models/match.h"

int match_get_winner_index(const Match* match) {
    if (match == NULL) return -1;

    // Il pareggio non esiste nel Tennis
    if (match->player_results[0].won_sets > match->player_results[1].won_sets) {
        return 0;
    }
    return 1;
}