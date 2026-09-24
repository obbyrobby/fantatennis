#include "../../include/models/team.h"

bool is_team_set(const Team* team) {
    for (int i = 0; i < TEAM_SIZE; i++) {
        if (team->players_ids[i] <= 0) {
            return false;
        }
    }
    return true;
}