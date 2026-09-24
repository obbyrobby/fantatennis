#ifndef FANTATENNIS_TEST_SERVICES_DATA_H
#define FANTATENNIS_TEST_SERVICES_DATA_H

#include "../include/models/user.h"
#include "../include/models/match.h"
#include "../include/models/tournament.h"
#include "../include/models/tennis_player.h"
#include "../include/models/team.h"

// 10 Tennisti
TennisPlayer test_players[10] = {
    { .name = "Jannik",    .surname = "Sinner",     .ranking = 1,  .is_available = true },
    { .name = "Carlos",    .surname = "Alcaraz",    .ranking = 2,  .is_available = true },
    { .name = "Alexander", .surname = "Zverev",     .ranking = 3,  .is_available = true },
    { .name = "Daniil",    .surname = "Medvedev",   .ranking = 4,  .is_available = true },
    { .name = "Novak",     .surname = "Djokovic",   .ranking = 5,  .is_available = true },
    { .name = "Lorenzo",   .surname = "Musetti",    .ranking = 6,  .is_available = true },
    { .name = "Matteo",    .surname = "Berrettini", .ranking = 7,  .is_available = true },
    { .name = "Taylor",    .surname = "Fritz",      .ranking = 8,  .is_available = true },
    { .name = "Ben",       .surname = "Shelton",    .ranking = 9,  .is_available = true },
    { .name = "Flavio",    .surname = "Cobolli",    .ranking = 10, .is_available = true }
};

// 2 Tornei
Tournament test_tournaments[2] = {
    { .title = "Torneo Test Open",   .state = TOURNAMENT_STATE_COMPLETED },
    { .title = "Torneo Test Master", .state = TOURNAMENT_STATE_OPEN }
};

// 20 partecipazioni
TournamentParticipation test_participations[20] = {
    // Torneo 1: Torneo Test Open (10 giocatori)
    { .tennisplayer_id = 1,  .tournament_id = 1 },
    { .tennisplayer_id = 2,  .tournament_id = 1 },
    { .tennisplayer_id = 3,  .tournament_id = 1 },
    { .tennisplayer_id = 4,  .tournament_id = 1 },
    { .tennisplayer_id = 5,  .tournament_id = 1 },
    { .tennisplayer_id = 6,  .tournament_id = 1 },
    { .tennisplayer_id = 7,  .tournament_id = 1 },
    { .tennisplayer_id = 8,  .tournament_id = 1 },
    { .tennisplayer_id = 9,  .tournament_id = 1 },
    { .tennisplayer_id = 10, .tournament_id = 1 },

    // Torneo 2: Torneo Test Master (10 giocatori)
    { .tennisplayer_id = 1,  .tournament_id = 2 },
    { .tennisplayer_id = 2,  .tournament_id = 2 },
    { .tennisplayer_id = 3,  .tournament_id = 2 },
    { .tennisplayer_id = 4,  .tournament_id = 2 },
    { .tennisplayer_id = 5,  .tournament_id = 2 },
    { .tennisplayer_id = 6,  .tournament_id = 2 },
    { .tennisplayer_id = 7,  .tournament_id = 2 },
    { .tennisplayer_id = 8,  .tournament_id = 2 },
    { .tennisplayer_id = 9,  .tournament_id = 2 },
    { .tennisplayer_id = 10, .tournament_id = 2 }
};

// 2 Utenti
User test_users[2] = {
    { .username = "tester_roby",  .password = "TestPass123!", .is_admin = false },
    { .username = "tester_tina",.password = "TestPass456!", .is_admin = false }
};

// Squadra
Team test_teams[1] = {
    // Team 1 - VALIDO (tester_roby su Torneo 1)
    // Djokovic (23) + Musetti (20) + Berrettini (17) + Fritz (14) + Shelton (11) + Cobolli (8) = 93/100 crediti
    {
        .tournament_id = 1,
        .user_id = 2,
        .players_ids = { 5, 6, 7, 8, 9, 10 }
    }
};

// 4 partite, 2 per torneo
Match test_matches[4] = {
    // Torneo 1: Torneo Test Open (2 partite)
    {
        .tournament_id = 1,
        .player_results = {
            { .player_id = 1, .won_sets = 3, .aces = 12, .double_faults = 2, .breaks = 4 }, // Sinner
            { .player_id = 2, .won_sets = 1, .aces = 7,  .double_faults = 3, .breaks = 1 }  // Alcaraz
        }
    },
    {
        .tournament_id = 1,
        .player_results = {
            { .player_id = 3, .won_sets = 3, .aces = 15, .double_faults = 4, .breaks = 3 }, // Zverev
            { .player_id = 4, .won_sets = 2, .aces = 9,  .double_faults = 5, .breaks = 2 }  // Medvedev
        }
    },

    // Torneo 2: Torneo Test Master (2 partite)
    {
        .tournament_id = 2,
        .player_results = {
            { .player_id = 5, .won_sets = 3, .aces = 8,  .double_faults = 1, .breaks = 5 }, // Djokovic
            { .player_id = 6, .won_sets = 0, .aces = 4,  .double_faults = 3, .breaks = 0 }  // Musetti
        }
    },
    {
        .tournament_id = 2,
        .player_results = {
            { .player_id = 7, .won_sets = 3, .aces = 18, .double_faults = 2, .breaks = 4 }, // Berrettini
            { .player_id = 3, .won_sets = 2, .aces = 14, .double_faults = 3, .breaks = 2 }  // Zverev
        }
    }
};

#endif
