#include "../../include/services/seeding_service.h"
#include "../../include/models/tennis_player.h"
#include "../../include/models/tournament.h"
#include "../../include/models/match.h"
#include "../../include/models/team.h"
#include "../../include/models/user.h"
#include "../../include/services/system_service.h"

void seed_system(AppContext* app_context) {
    // Cancelliamo tutto prima di iniziare. Così facendo, preveniamo duplicati o foreign key che puntano agli ID sbagliati
    clear_system(app_context);

    // Seeding dei Tennisti
    TennisPlayer players[50] = {
        { .name = "Jannik", .surname = "Sinner", .ranking = 1, .is_available = true },
        { .name = "Alexander", .surname = "Zverev", .ranking = 2, .is_available = true },
        { .name = "Carlos", .surname = "Alcaraz", .ranking = 3, .is_available = true },
        { .name = "Felix", .surname = "Auger-Aliassime", .ranking = 4, .is_available = true },
        { .name = "Novak", .surname = "Djokovic", .ranking = 5, .is_available = true },
        { .name = "Alex", .surname = "de Minaur", .ranking = 6, .is_available = true },
        { .name = "Daniil", .surname = "Medvedev", .ranking = 7, .is_available = true },
        { .name = "Ben", .surname = "Shelton", .ranking = 8, .is_available = true },
        { .name = "Flavio", .surname = "Cobolli", .ranking = 9, .is_available = true },
        { .name = "Taylor", .surname = "Fritz", .ranking = 10, .is_available = true },
        { .name = "Alexander", .surname = "Bublik", .ranking = 11, .is_available = true },
        { .name = "Jiri", .surname = "Lehecka", .ranking = 12, .is_available = true },
        { .name = "Casper", .surname = "Ruud", .ranking = 13, .is_available = true },
        { .name = "Andrey", .surname = "Rublev", .ranking = 14, .is_available = true },
        { .name = "Lorenzo", .surname = "Musetti", .ranking = 15, .is_available = true },
        { .name = "Learner", .surname = "Tien", .ranking = 16, .is_available = true },
        { .name = "Jakub", .surname = "Mensik", .ranking = 17, .is_available = true },
        { .name = "Valentin", .surname = "Vacherot", .ranking = 18, .is_available = true },
        { .name = "Frances", .surname = "Tiafoe", .ranking = 19, .is_available = true },
        { .name = "Francisco", .surname = "Cerundolo", .ranking = 20, .is_available = true },
        { .name = "Tommy", .surname = "Paul", .ranking = 21, .is_available = true },
        { .name = "Arthur", .surname = "Fils", .ranking = 22, .is_available = true },
        { .name = "Luciano", .surname = "Darderi", .ranking = 23, .is_available = true },
        { .name = "Rafael", .surname = "Jodar", .ranking = 24, .is_available = true },
        { .name = "Alejandro", .surname = "Davidovich Fokina", .ranking = 25, .is_available = true },
        { .name = "Karen", .surname = "Khachanov", .ranking = 26, .is_available = true },
        { .name = "Joao", .surname = "Fonseca", .ranking = 27, .is_available = true },
        { .name = "Arthur", .surname = "Rinderknech", .ranking = 28, .is_available = true },
        { .name = "Ugo", .surname = "Humbert", .ranking = 29, .is_available = true },
        { .name = "Alejandro", .surname = "Tabilo", .ranking = 30, .is_available = true },
        { .name = "Tomas Martin", .surname = "Etcheverry", .ranking = 31, .is_available = true },
        { .name = "Alexander", .surname = "Blockx", .ranking = 32, .is_available = true },
        { .name = "Brandon", .surname = "Nakashima", .ranking = 33, .is_available = true },
        { .name = "Ignacio", .surname = "Buse", .ranking = 34, .is_available = true },
        { .name = "Matteo", .surname = "Arnaldi", .ranking = 35, .is_available = true },
        { .name = "Zizou", .surname = "Bergs", .ranking = 36, .is_available = true },
        { .name = "Arthur", .surname = "Fery", .ranking = 37, .is_available = true },
        { .name = "Raphael", .surname = "Collignon", .ranking = 38, .is_available = true },
        { .name = "Cameron", .surname = "Norrie", .ranking = 39, .is_available = true },
        { .name = "Alex", .surname = "Michelsen", .ranking = 40, .is_available = true },
        { .name = "Matteo", .surname = "Berrettini", .ranking = 41, .is_available = true },
        { .name = "Jan-Lennard", .surname = "Struff", .ranking = 42, .is_available = true },
        { .name = "Jaume", .surname = "Munar", .ranking = 43, .is_available = true },
        { .name = "Mariano", .surname = "Navone", .ranking = 44, .is_available = true },
        { .name = "Yannick", .surname = "Hanfmann", .ranking = 45, .is_available = true },
        { .name = "Ethan", .surname = "Quinn", .ranking = 46, .is_available = true },
        { .name = "Adrian", .surname = "Mannarino", .ranking = 47, .is_available = true },
        { .name = "Luca", .surname = "Van Assche", .ranking = 48, .is_available = true },
        { .name = "Nuno", .surname = "Borges", .ranking = 49, .is_available = true },
        { .name = "Juan Manuel", .surname = "Cerundolo", .ranking = 50, .is_available = true }
    };

    // Seeding dei Tornei
    Tournament tournaments[4] = {
        { .title = "Roland-Garros",   .state = TOURNAMENT_STATE_COMPLETED, .winner_id = 23 },
        { .title = "Australian Open", .state = TOURNAMENT_STATE_COMPLETED, .winner_id = 2 },
        { .title = "Wimbledon",       .state = TOURNAMENT_STATE_ONGOING },
        { .title = "US Open",         .state = TOURNAMENT_STATE_OPEN }
    };

    TournamentParticipation participations[110] = {
    // Torneo 1: Roland-Garros (28 giocatori)
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
    { .tennisplayer_id = 11, .tournament_id = 1 },
    { .tennisplayer_id = 12, .tournament_id = 1 },
    { .tennisplayer_id = 13, .tournament_id = 1 },
    { .tennisplayer_id = 14, .tournament_id = 1 },
    { .tennisplayer_id = 15, .tournament_id = 1 },
    { .tennisplayer_id = 16, .tournament_id = 1 },
    { .tennisplayer_id = 17, .tournament_id = 1 },
    { .tennisplayer_id = 19, .tournament_id = 1 },
    { .tennisplayer_id = 20, .tournament_id = 1 },
    { .tennisplayer_id = 22, .tournament_id = 1 },
    { .tennisplayer_id = 23, .tournament_id = 1 },
    { .tennisplayer_id = 25, .tournament_id = 1 },
    { .tennisplayer_id = 26, .tournament_id = 1 },
    { .tennisplayer_id = 28, .tournament_id = 1 },
    { .tennisplayer_id = 29, .tournament_id = 1 },
    { .tennisplayer_id = 30, .tournament_id = 1 },
    { .tennisplayer_id = 43, .tournament_id = 1 },
    { .tennisplayer_id = 44, .tournament_id = 1 },

    // Torneo 2: Australian Open (26 giocatori)
    { .tennisplayer_id = 1,  .tournament_id = 2 },
    { .tennisplayer_id = 2,  .tournament_id = 2 },
    { .tennisplayer_id = 3,  .tournament_id = 2 },
    { .tennisplayer_id = 5,  .tournament_id = 2 },
    { .tennisplayer_id = 6,  .tournament_id = 2 },
    { .tennisplayer_id = 7,  .tournament_id = 2 },
    { .tennisplayer_id = 8,  .tournament_id = 2 },
    { .tennisplayer_id = 10, .tournament_id = 2 },
    { .tennisplayer_id = 11, .tournament_id = 2 },
    { .tennisplayer_id = 13, .tournament_id = 2 },
    { .tennisplayer_id = 14, .tournament_id = 2 },
    { .tennisplayer_id = 15, .tournament_id = 2 },
    { .tennisplayer_id = 16, .tournament_id = 2 },
    { .tennisplayer_id = 17, .tournament_id = 2 },
    { .tennisplayer_id = 19, .tournament_id = 2 },
    { .tennisplayer_id = 21, .tournament_id = 2 },
    { .tennisplayer_id = 22, .tournament_id = 2 },
    { .tennisplayer_id = 24, .tournament_id = 2 },
    { .tennisplayer_id = 26, .tournament_id = 2 },
    { .tennisplayer_id = 27, .tournament_id = 2 },
    { .tennisplayer_id = 28, .tournament_id = 2 },
    { .tennisplayer_id = 32, .tournament_id = 2 },
    { .tennisplayer_id = 36, .tournament_id = 2 },
    { .tennisplayer_id = 39, .tournament_id = 2 },
    { .tennisplayer_id = 40, .tournament_id = 2 },
    { .tennisplayer_id = 49, .tournament_id = 2 },

    // Torneo 3: Wimbledon (26 giocatori)
    { .tennisplayer_id = 1,  .tournament_id = 3 },
    { .tennisplayer_id = 2,  .tournament_id = 3 },
    { .tennisplayer_id = 3,  .tournament_id = 3 },
    { .tennisplayer_id = 4,  .tournament_id = 3 },
    { .tennisplayer_id = 5,  .tournament_id = 3 },
    { .tennisplayer_id = 6,  .tournament_id = 3 },
    { .tennisplayer_id = 7,  .tournament_id = 3 },
    { .tennisplayer_id = 8,  .tournament_id = 3 },
    { .tennisplayer_id = 9,  .tournament_id = 3 },
    { .tennisplayer_id = 10, .tournament_id = 3 },
    { .tennisplayer_id = 11, .tournament_id = 3 },
    { .tennisplayer_id = 12, .tournament_id = 3 },
    { .tennisplayer_id = 14, .tournament_id = 3 },
    { .tennisplayer_id = 15, .tournament_id = 3 },
    { .tennisplayer_id = 17, .tournament_id = 3 },
    { .tennisplayer_id = 19, .tournament_id = 3 },
    { .tennisplayer_id = 21, .tournament_id = 3 },
    { .tennisplayer_id = 22, .tournament_id = 3 },
    { .tennisplayer_id = 26, .tournament_id = 3 },
    { .tennisplayer_id = 28, .tournament_id = 3 },
    { .tennisplayer_id = 29, .tournament_id = 3 },
    { .tennisplayer_id = 32, .tournament_id = 3 },
    { .tennisplayer_id = 36, .tournament_id = 3 },
    { .tennisplayer_id = 37, .tournament_id = 3 },
    { .tennisplayer_id = 41, .tournament_id = 3 },
    { .tennisplayer_id = 42, .tournament_id = 3 },

    // Torneo 4: US Open (30 giocatori)
    { .tennisplayer_id = 1,  .tournament_id = 4 },
    { .tennisplayer_id = 2,  .tournament_id = 4 },
    { .tennisplayer_id = 3,  .tournament_id = 4 },
    { .tennisplayer_id = 4,  .tournament_id = 4 },
    { .tennisplayer_id = 5,  .tournament_id = 4 },
    { .tennisplayer_id = 6,  .tournament_id = 4 },
    { .tennisplayer_id = 7,  .tournament_id = 4 },
    { .tennisplayer_id = 8,  .tournament_id = 4 },
    { .tennisplayer_id = 9,  .tournament_id = 4 },
    { .tennisplayer_id = 10, .tournament_id = 4 },
    { .tennisplayer_id = 11, .tournament_id = 4 },
    { .tennisplayer_id = 12, .tournament_id = 4 },
    { .tennisplayer_id = 13, .tournament_id = 4 },
    { .tennisplayer_id = 14, .tournament_id = 4 },
    { .tennisplayer_id = 15, .tournament_id = 4 },
    { .tennisplayer_id = 16, .tournament_id = 4 },
    { .tennisplayer_id = 17, .tournament_id = 4 },
    { .tennisplayer_id = 18, .tournament_id = 4 },
    { .tennisplayer_id = 19, .tournament_id = 4 },
    { .tennisplayer_id = 20, .tournament_id = 4 },
    { .tennisplayer_id = 21, .tournament_id = 4 },
    { .tennisplayer_id = 23, .tournament_id = 4 },
    { .tennisplayer_id = 24, .tournament_id = 4 },
    { .tennisplayer_id = 26, .tournament_id = 4 },
    { .tennisplayer_id = 27, .tournament_id = 4 },
    { .tennisplayer_id = 28, .tournament_id = 4 },
    { .tennisplayer_id = 29, .tournament_id = 4 },
    { .tennisplayer_id = 32, .tournament_id = 4 },
    { .tennisplayer_id = 38, .tournament_id = 4 },
    { .tennisplayer_id = 42, .tournament_id = 4 }
};

Match matches[40] = {
    // Torneo 1: Roland-Garros (16 partite)
    {
        .tournament_id = 1,
        .player_results = {
            { .player_id = 3, .won_sets = 3, .aces = 6,  .double_faults = 3, .breaks = 5 }, // Carlos Alcaraz
            { .player_id = 2, .won_sets = 2, .aces = 14, .double_faults = 4, .breaks = 3 }  // Alexander Zverev
        }
    },
    {
        .tournament_id = 1,
        .player_results = {
            { .player_id = 1, .won_sets = 3, .aces = 11, .double_faults = 2, .breaks = 4 }, // Jannik Sinner
            { .player_id = 5, .won_sets = 1, .aces = 7,  .double_faults = 3, .breaks = 2 }  // Novak Djokovic
        }
    },
    {
        .tournament_id = 1,
        .player_results = {
            { .player_id = 13, .won_sets = 3, .aces = 8, .double_faults = 1, .breaks = 6 }, // Casper Ruud
            { .player_id = 8,  .won_sets = 0, .aces = 5, .double_faults = 5, .breaks = 1 }  // Ben Shelton
        }
    },
    {
        .tournament_id = 1,
        .player_results = {
            { .player_id = 15, .won_sets = 3, .aces = 10, .double_faults = 2, .breaks = 4 }, // Lorenzo Musetti
            { .player_id = 10, .won_sets = 2, .aces = 12, .double_faults = 4, .breaks = 3 }  // Taylor Fritz
        }
    },
    {
        .tournament_id = 1,
        .player_results = {
            { .player_id = 20, .won_sets = 3, .aces = 4, .double_faults = 2, .breaks = 5 }, // Francisco Cerundolo
            { .player_id = 25, .won_sets = 1, .aces = 3, .double_faults = 6, .breaks = 2 }  // Alejandro Davidovich Fokina
        }
    },
    {
        .tournament_id = 1,
        .player_results = {
            { .player_id = 4,  .won_sets = 3, .aces = 13, .double_faults = 3, .breaks = 4 }, // Felix Auger-Aliassime
            { .player_id = 17, .won_sets = 1, .aces = 8,  .double_faults = 5, .breaks = 1 }  // Jakub Mensik
        }
    },
    {
        .tournament_id = 1,
        .player_results = {
            { .player_id = 6,  .won_sets = 3, .aces = 5, .double_faults = 1, .breaks = 6 }, // Alex de Minaur
            { .player_id = 23, .won_sets = 0, .aces = 4, .double_faults = 4, .breaks = 1 }  // Luciano Darderi
        }
    },
    {
        .tournament_id = 1,
        .player_results = {
            { .player_id = 7,  .won_sets = 3, .aces = 12, .double_faults = 5, .breaks = 4 }, // Daniil Medvedev
            { .player_id = 28, .won_sets = 1, .aces = 7,  .double_faults = 3, .breaks = 2 }  // Arthur Rinderknech
        }
    },
    {
        .tournament_id = 1,
        .player_results = {
            { .player_id = 9,  .won_sets = 3, .aces = 9,  .double_faults = 2, .breaks = 5 }, // Flavio Cobolli
            { .player_id = 29, .won_sets = 2, .aces = 11, .double_faults = 4, .breaks = 3 }  // Ugo Humbert
        }
    },
    {
        .tournament_id = 1,
        .player_results = {
            { .player_id = 11, .won_sets = 3, .aces = 15, .double_faults = 6, .breaks = 3 }, // Alexander Bublik
            { .player_id = 30, .won_sets = 1, .aces = 6,  .double_faults = 2, .breaks = 2 }  // Alejandro Tabilo
        }
    },
    {
        .tournament_id = 1,
        .player_results = {
            { .player_id = 12, .won_sets = 3, .aces = 10, .double_faults = 3, .breaks = 4 }, // Jiri Lehecka
            { .player_id = 43, .won_sets = 0, .aces = 3,  .double_faults = 3, .breaks = 1 }  // Jaume Munar
        }
    },
    {
        .tournament_id = 1,
        .player_results = {
            { .player_id = 14, .won_sets = 3, .aces = 12, .double_faults = 2, .breaks = 5 }, // Andrey Rublev
            { .player_id = 44, .won_sets = 1, .aces = 2,  .double_faults = 3, .breaks = 1 }  // Mariano Navone
        }
    },
    {
        .tournament_id = 1,
        .player_results = {
            { .player_id = 19, .won_sets = 3, .aces = 14, .double_faults = 4, .breaks = 4 }, // Frances Tiafoe
            { .player_id = 16, .won_sets = 2, .aces = 5,  .double_faults = 1, .breaks = 3 }  // Learner Tien
        }
    },
    {
        .tournament_id = 1,
        .player_results = {
            { .player_id = 22, .won_sets = 3, .aces = 8, .double_faults = 3, .breaks = 4 }, // Arthur Fils
            { .player_id = 26, .won_sets = 0, .aces = 9, .double_faults = 4, .breaks = 1 }  // Karen Khachanov
        }
    },
    {
        .tournament_id = 1,
        .player_results = {
            { .player_id = 1,  .won_sets = 3, .aces = 13, .double_faults = 2, .breaks = 5 }, // Jannik Sinner
            { .player_id = 13, .won_sets = 1, .aces = 7,  .double_faults = 2, .breaks = 2 }  // Casper Ruud
        }
    },
    {
        .tournament_id = 1,
        .player_results = {
            { .player_id = 3, .won_sets = 3, .aces = 8,  .double_faults = 3, .breaks = 4 }, // Carlos Alcaraz
            { .player_id = 1, .won_sets = 2, .aces = 12, .double_faults = 2, .breaks = 3 }  // Jannik Sinner
        }
    },

    // Torneo 2: Australian Open (16 partite)
    {
        .tournament_id = 2,
        .player_results = {
            { .player_id = 1, .won_sets = 3, .aces = 15, .double_faults = 1, .breaks = 5 }, // Jannik Sinner
            { .player_id = 7, .won_sets = 2, .aces = 18, .double_faults = 7, .breaks = 2 }  // Daniil Medvedev
        }
    },
    {
        .tournament_id = 2,
        .player_results = {
            { .player_id = 5,  .won_sets = 3, .aces = 12, .double_faults = 2, .breaks = 4 }, // Novak Djokovic
            { .player_id = 10, .won_sets = 1, .aces = 14, .double_faults = 3, .breaks = 1 }  // Taylor Fritz
        }
    },
    {
        .tournament_id = 2,
        .player_results = {
            { .player_id = 6,  .won_sets = 3, .aces = 9,  .double_faults = 2, .breaks = 5 }, // Alex de Minaur
            { .player_id = 14, .won_sets = 1, .aces = 11, .double_faults = 4, .breaks = 2 }  // Andrey Rublev
        }
    },
    {
        .tournament_id = 2,
        .player_results = {
            { .player_id = 27, .won_sets = 3, .aces = 7,  .double_faults = 2, .breaks = 4 }, // Joao Fonseca
            { .player_id = 21, .won_sets = 2, .aces = 10, .double_faults = 3, .breaks = 3 }  // Tommy Paul
        }
    },
    {
        .tournament_id = 2,
        .player_results = {
            { .player_id = 2,  .won_sets = 3, .aces = 16, .double_faults = 3, .breaks = 4 }, // Alexander Zverev
            { .player_id = 16, .won_sets = 0, .aces = 4,  .double_faults = 2, .breaks = 1 }  // Learner Tien
        }
    },
    {
        .tournament_id = 2,
        .player_results = {
            { .player_id = 3,  .won_sets = 3, .aces = 10, .double_faults = 3, .breaks = 5 }, // Carlos Alcaraz
            { .player_id = 24, .won_sets = 0, .aces = 5,  .double_faults = 4, .breaks = 1 }  // Rafael Jodar
        }
    },
    {
        .tournament_id = 2,
        .player_results = {
            { .player_id = 8,  .won_sets = 3, .aces = 19, .double_faults = 5, .breaks = 3 }, // Ben Shelton
            { .player_id = 32, .won_sets = 1, .aces = 8,  .double_faults = 3, .breaks = 1 }  // Alexander Blockx
        }
    },
    {
        .tournament_id = 2,
        .player_results = {
            { .player_id = 11, .won_sets = 3, .aces = 21, .double_faults = 7, .breaks = 4 }, // Alexander Bublik
            { .player_id = 36, .won_sets = 2, .aces = 9,  .double_faults = 2, .breaks = 3 }  // Zizou Bergs
        }
    },
    {
        .tournament_id = 2,
        .player_results = {
            { .player_id = 13, .won_sets = 3, .aces = 11, .double_faults = 1, .breaks = 4 }, // Casper Ruud
            { .player_id = 39, .won_sets = 0, .aces = 3,  .double_faults = 3, .breaks = 0 }  // Cameron Norrie
        }
    },
    {
        .tournament_id = 2,
        .player_results = {
            { .player_id = 15, .won_sets = 3, .aces = 8, .double_faults = 2, .breaks = 5 }, // Lorenzo Musetti
            { .player_id = 40, .won_sets = 1, .aces = 7, .double_faults = 4, .breaks = 2 }  // Alex Michelsen
        }
    },
    {
        .tournament_id = 2,
        .player_results = {
            { .player_id = 17, .won_sets = 3, .aces = 14, .double_faults = 4, .breaks = 4 }, // Jakub Mensik
            { .player_id = 49, .won_sets = 1, .aces = 4,  .double_faults = 2, .breaks = 1 }  // Nuno Borges
        }
    },
    {
        .tournament_id = 2,
        .player_results = {
            { .player_id = 19, .won_sets = 3, .aces = 13, .double_faults = 3, .breaks = 4 }, // Frances Tiafoe
            { .player_id = 28, .won_sets = 2, .aces = 10, .double_faults = 5, .breaks = 2 }  // Arthur Rinderknech
        }
    },
    {
        .tournament_id = 2,
        .player_results = {
            { .player_id = 22, .won_sets = 3, .aces = 9, .double_faults = 2, .breaks = 4 }, // Arthur Fils
            { .player_id = 26, .won_sets = 1, .aces = 6, .double_faults = 3, .breaks = 2 }  // Karen Khachanov
        }
    },
    {
        .tournament_id = 2,
        .player_results = {
            { .player_id = 1, .won_sets = 3, .aces = 14, .double_faults = 1, .breaks = 4 }, // Jannik Sinner
            { .player_id = 5, .won_sets = 1, .aces = 9,  .double_faults = 2, .breaks = 2 }  // Novak Djokovic
        }
    },
    {
        .tournament_id = 2,
        .player_results = {
            { .player_id = 7, .won_sets = 3, .aces = 16, .double_faults = 4, .breaks = 3 }, // Daniil Medvedev
            { .player_id = 2, .won_sets = 2, .aces = 15, .double_faults = 5, .breaks = 3 }  // Alexander Zverev
        }
    },
    {
        .tournament_id = 2,
        .player_results = {
            { .player_id = 1, .won_sets = 3, .aces = 17, .double_faults = 2, .breaks = 4 }, // Jannik Sinner
            { .player_id = 3, .won_sets = 1, .aces = 8,  .double_faults = 3, .breaks = 2 }  // Carlos Alcaraz
        }
    },

    // Torneo 3: Wimbledon (8 partite)
    {
        .tournament_id = 3,
        .player_results = {
            { .player_id = 3, .won_sets = 3, .aces = 14, .double_faults = 2, .breaks = 5 }, // Carlos Alcaraz
            { .player_id = 5, .won_sets = 0, .aces = 8,  .double_faults = 3, .breaks = 0 }  // Novak Djokovic
        }
    },
    {
        .tournament_id = 3,
        .player_results = {
            { .player_id = 7, .won_sets = 3, .aces = 13, .double_faults = 4, .breaks = 4 }, // Daniil Medvedev
            { .player_id = 1, .won_sets = 2, .aces = 15, .double_faults = 3, .breaks = 3 }  // Jannik Sinner
        }
    },
    {
        .tournament_id = 3,
        .player_results = {
            { .player_id = 11, .won_sets = 3, .aces = 22, .double_faults = 8, .breaks = 3 }, // Alexander Bublik
            { .player_id = 8,  .won_sets = 2, .aces = 19, .double_faults = 5, .breaks = 2 }  // Ben Shelton
        }
    },
    {
        .tournament_id = 3,
        .player_results = {
            { .player_id = 41, .won_sets = 3, .aces = 17, .double_faults = 1, .breaks = 3 }, // Matteo Berrettini
            { .player_id = 19, .won_sets = 1, .aces = 12, .double_faults = 3, .breaks = 1 }  // Frances Tiafoe
        }
    },
    {
        .tournament_id = 3,
        .player_results = {
            { .player_id = 4,  .won_sets = 3, .aces = 16, .double_faults = 4, .breaks = 4 }, // Felix Auger-Aliassime
            { .player_id = 28, .won_sets = 1, .aces = 9,  .double_faults = 3, .breaks = 1 }  // Arthur Rinderknech
        }
    },
    {
        .tournament_id = 3,
        .player_results = {
            { .player_id = 2,  .won_sets = 3, .aces = 18, .double_faults = 2, .breaks = 5 }, // Alexander Zverev
            { .player_id = 32, .won_sets = 0, .aces = 5,  .double_faults = 3, .breaks = 0 }  // Alexander Blockx
        }
    },
    {
        .tournament_id = 3,
        .player_results = {
            { .player_id = 6,  .won_sets = 3, .aces = 10, .double_faults = 1, .breaks = 4 }, // Alex de Minaur
            { .player_id = 37, .won_sets = 2, .aces = 11, .double_faults = 4, .breaks = 3 }  // Arthur Fery
        }
    },
    {
        .tournament_id = 3,
        .player_results = {
            { .player_id = 10, .won_sets = 3, .aces = 15, .double_faults = 2, .breaks = 4 }, // Taylor Fritz
            { .player_id = 42, .won_sets = 1, .aces = 13, .double_faults = 3, .breaks = 2 }  // Jan-Lennard Struff
        }
    }

    // Torneo 4: US Open (0 partite - torneo non ancora iniziato)
    };

    // Seeding degli Utenti (10 utenti registrati)
    User users[10] = {
        { .username = "shes_matrix",         .password = "lampione80",        .is_admin = false },
        { .username = "alexis",              .password = "presaJe$",          .is_admin = false },
        { .username = "fantatennis_roma",    .password = "DajeRoma1927!",     .is_admin = false },
        { .username = "smash_calabrese",     .password = "Peperoncino22*",    .is_admin = false },
        { .username = "rovescio_lungolinea", .password = "SliceWinner88!",    .is_admin = false },
        { .username = "volee_sotto_rete",    .password = "TieBreakCampione#", .is_admin = false },
        { .username = "er_martello_de_roma", .password = "ForzaMatteo!41",    .is_admin = false },
        { .username = "pallacorda_magica",   .password = "AceCentrale77$",    .is_admin = false },
        { .username = "match_point_milan",   .password = "SanSiroTennis1!",   .is_admin = false },
        { .username = "roberto_scemo",       .password = "rob_vai_a_lavora'", .is_admin = false }
    };

    // Seeding dei Team (budget max: 100 crediti, 6 giocatori per team, 6 team per torneo)
    // Nota: user_id parte da 2 (user_id = 1 riservato all'account admin)
    Team teams[24] = {
    // =========================================================================
    // Torneo 1: Roland-Garros (Tournament ID 1) - 28 Partecipanti
    // =========================================================================

    // Team 1 - User 2 (shes_matrix) | Costo totale: 96/100 crediti
    // Sinner (35) + Fils (16) + Darderi (15) + Rinderknech (12) + Tabilo (10) + Navone (8)
    {
        .tournament_id = 1,
        .user_id = 2,
        .players_ids = { 1, 22, 23, 28, 30, 44 }
    },

    // Team 2 - User 3 (alexis) | Costo totale: 97/100 crediti
    // Alcaraz (33) + Cerundolo (17) + Davidovich Fokina (14) + Khachanov (13) + Humbert (11) + Munar (9)
    {
        .tournament_id = 1,
        .user_id = 3,
        .players_ids = { 3, 20, 25, 26, 29, 43 }
    },

    // Team 3 - User 4 (fantatennis_roma) | Costo totale: 99/100 crediti
    // Lehecka (24) + Rublev (22) + Mensik (19) + Darderi (15) + Humbert (11) + Navone (8)
    {
        .tournament_id = 1,
        .user_id = 4,
        .players_ids = { 12, 14, 17, 23, 29, 44 }
    },

    // Team 4 - User 5 (smash_calabrese) | Costo totale: 100/100 crediti
    // Djokovic (31) + Musetti (21) + Tiafoe (18) + Rinderknech (12) + Tabilo (10) + Navone (8)
    {
        .tournament_id = 1,
        .user_id = 5,
        .players_ids = { 5, 15, 19, 28, 30, 44 }
    },

    // Team 5 - User 6 (rovescio_lungolinea) | Costo totale: 100/100 crediti
    // de Minaur (30) + Rublev (22) + Tiafoe (18) + Khachanov (13) + Munar (9) + Navone (8)
    {
        .tournament_id = 1,
        .user_id = 6,
        .players_ids = { 6, 14, 19, 26, 43, 44 }
    },

    // Team 6 - User 7 (volee_sotto_rete) | Costo totale: 100/100 crediti
    // Cobolli (27) + Lehecka (24) + Cerundolo (17) + Darderi (15) + Munar (9) + Navone (8)
    {
        .tournament_id = 1,
        .user_id = 7,
        .players_ids = { 9, 12, 20, 23, 43, 44 }
    },

    // =========================================================================
    // Torneo 2: Australian Open (Tournament ID 2) - 26 Partecipanti
    // =========================================================================

    // Team 7 - User 2 (shes_matrix) | Costo totale: 99/100 crediti
    // Sinner (35) + Jodar (17) + Fonseca (15) + Blockx (13) + Norrie (11) + Borges (8)
    {
        .tournament_id = 2,
        .user_id = 2,
        .players_ids = { 1, 24, 27, 32, 39, 49 }
    },

    // Team 8 - User 8 (er_martello_de_roma) | Costo totale: 100/100 crediti
    // Zverev (34) + Fils (18) + Khachanov (16) + Rinderknech (14) + Michelsen (10) + Borges (8)
    {
        .tournament_id = 2,
        .user_id = 8,
        .players_ids = { 2, 22, 26, 28, 40, 49 }
    },

    // Team 9 - User 9 (pallacorda_magica) | Costo totale: 96/100 crediti
    // Djokovic (32) + Mensik (21) + Blockx (13) + Bergs (12) + Michelsen (10) + Borges (8)
    {
        .tournament_id = 2,
        .user_id = 9,
        .players_ids = { 5, 17, 32, 36, 40, 49 }
    },

    // Team 10 - User 10 (match_point_milan) | Costo totale: 100/100 crediti
    // Medvedev (30) + Mensik (21) + Paul (19) + Bergs (12) + Michelsen (10) + Borges (8)
    {
        .tournament_id = 2,
        .user_id = 10,
        .players_ids = { 7, 17, 21, 36, 40, 49 }
    },

    // Team 11 - User 11 (roberto_scemo) | Costo totale: 100/100 crediti
    // Shelton (29) + Rublev (25) + Fonseca (15) + Blockx (13) + Michelsen (10) + Borges (8)
    {
        .tournament_id = 2,
        .user_id = 11,
        .players_ids = { 8, 14, 27, 32, 40, 49 }
    },

    // Team 12 - User 4 (fantatennis_roma) | Costo totale: 99/100 crediti
    // Fritz (28) + Rublev (25) + Khachanov (16) + Bergs (12) + Michelsen (10) + Borges (8)
    {
        .tournament_id = 2,
        .user_id = 4,
        .players_ids = { 10, 14, 26, 36, 40, 49 }
    },

    // =========================================================================
    // Torneo 3: Wimbledon (Tournament ID 3) - 26 Partecipanti
    // =========================================================================

    // Team 13 - User 3 (alexis) | Costo totale: 100/100 crediti
    // Alcaraz (33) + Fils (17) + Khachanov (16) + Rinderknech (15) + Fery (11) + Struff (8)
    {
        .tournament_id = 3,
        .user_id = 3,
        .players_ids = { 3, 22, 26, 28, 37, 42 }
    },

    // Team 14 - User 7 (volee_sotto_rete) | Costo totale: 99/100 crediti
    // Medvedev (29) + Bublik (25) + Humbert (14) + Blockx (13) + Berrettini (10) + Struff (8)
    {
        .tournament_id = 3,
        .user_id = 7,
        .players_ids = { 7, 11, 29, 32, 41, 42 }
    },

    // Team 15 - User 5 (smash_calabrese) | Costo totale: 98/100 crediti
    // Auger-Aliassime (32) + Mensik (20) + Khachanov (16) + Bergs (12) + Berrettini (10) + Struff (8)
    {
        .tournament_id = 3,
        .user_id = 5,
        .players_ids = { 4, 17, 26, 36, 41, 42 }
    },

    // Team 16 - User 8 (er_martello_de_roma) | Costo totale: 100/100 crediti
    // Sinner (35) + Tiafoe (19) + Rinderknech (15) + Blockx (13) + Berrettini (10) + Struff (8)
    {
        .tournament_id = 3,
        .user_id = 8,
        .players_ids = { 1, 19, 28, 32, 41, 42 }
    },

    // Team 17 - User 9 (pallacorda_magica) | Costo totale: 99/100 crediti
    // Shelton (28) + Lehecka (24) + Paul (18) + Fery (11) + Berrettini (10) + Struff (8)
    {
        .tournament_id = 3,
        .user_id = 9,
        .players_ids = { 8, 12, 21, 37, 41, 42 }
    },

    // Team 18 - User 10 (match_point_milan) | Costo totale: 99/100 crediti
    // de Minaur (30) + Musetti (21) + Fils (17) + Blockx (13) + Berrettini (10) + Struff (8)
    {
        .tournament_id = 3,
        .user_id = 10,
        .players_ids = { 6, 15, 22, 32, 41, 42 }
    },

    // =========================================================================
    // Torneo 4: US Open (Tournament ID 4) - 30 Partecipanti
    // =========================================================================

    // Team 19 - User 4 (fantatennis_roma) | Costo totale: 98/100 crediti
    // Sinner (35) + Paul (17) + Jodar (15) + Fonseca (13) + Blockx (10) + Struff (8)
    {
        .tournament_id = 4,
        .user_id = 4,
        .players_ids = { 1, 21, 24, 27, 32, 42 }
    },

    // Team 20 - User 6 (rovescio_lungolinea) | Costo totale: 99/100 crediti
    // Shelton (29) + Lehecka (25) + Darderi (16) + Rinderknech (12) + Collignon (9) + Struff (8)
    {
        .tournament_id = 4,
        .user_id = 6,
        .players_ids = { 8, 12, 23, 28, 38, 42 }
    },

    // Team 21 - User 11 (roberto_scemo) | Costo totale: 96/100 crediti
    // Alcaraz (34) + Cerundolo (18) + Khachanov (14) + Fonseca (13) + Collignon (9) + Struff (8)
    {
        .tournament_id = 4,
        .user_id = 11,
        .players_ids = { 3, 20, 26, 27, 38, 42 }
    },

    // Team 22 - User 2 (shes_matrix) | Costo totale: 100/100 crediti
    // Zverev (35) + Vacherot (20) + Darderi (16) + Rinderknech (12) + Collignon (9) + Struff (8)
    {
        .tournament_id = 4,
        .user_id = 2,
        .players_ids = { 2, 18, 23, 28, 38, 42 }
    },

    // Team 23 - User 3 (alexis) | Costo totale: 99/100 crediti
    // Cobolli (28) + Musetti (22) + Paul (17) + Jodar (15) + Collignon (9) + Struff (8)
    {
        .tournament_id = 4,
        .user_id = 3,
        .players_ids = { 9, 15, 21, 24, 38, 42 }
    },

    // Team 24 - User 8 (er_martello_de_roma) | Costo totale: 100/100 crediti
    // Medvedev (30) + Rublev (23) + Tiafoe (19) + Humbert (11) + Collignon (9) + Struff (8)
    {
        .tournament_id = 4,
        .user_id = 8,
        .players_ids = { 7, 14, 19, 29, 38, 42 }
    }
    };

    entity_table_batch_begin(&app_context->matches_table);
    entity_table_batch_begin(&app_context->teams_table);
    entity_table_batch_begin(&app_context->participations_table);
    entity_table_batch_begin(&app_context->tennisplayers_table);
    entity_table_batch_begin(&app_context->users_table);
    entity_table_batch_begin(&app_context->tournaments_table);

    int players_count = sizeof(players) / sizeof(players[0]);
    for (int i = 0; i < players_count; i++) {
        entity_table_add_item(&app_context->tennisplayers_table, &players[i]);
    }

    int tournaments_count = sizeof(tournaments) / sizeof(tournaments[0]);
    for (int i = 0; i < tournaments_count; i++) {
        entity_table_add_item(&app_context->tournaments_table, &tournaments[i]);
    }

    int participations_count = sizeof(participations) / sizeof(participations[0]);
    for (int i = 0; i < participations_count; i++) {
        entity_table_add_item(&app_context->participations_table, &participations[i]);
    }

    int matches_count = sizeof(matches) / sizeof(matches[0]);
    for (int i = 0; i < matches_count; i++) {
        entity_table_add_item(&app_context->matches_table, &matches[i]);
    }

    int users_count = sizeof(users) / sizeof(users[0]);
    for (int i = 0; i < users_count; i++) {
        entity_table_add_item(&app_context->users_table, &users[i]);
    }

    int teams_count = sizeof(teams) / sizeof(teams[0]);
    for (int i = 0; i < teams_count; i++) {
        entity_table_add_item(&app_context->teams_table, &teams[i]);
    }

    entity_table_batch_commit(&app_context->matches_table);
    entity_table_batch_commit(&app_context->teams_table);
    entity_table_batch_commit(&app_context->participations_table);
    entity_table_batch_commit(&app_context->tennisplayers_table);
    entity_table_batch_commit(&app_context->users_table);
    entity_table_batch_commit(&app_context->tournaments_table);
}
