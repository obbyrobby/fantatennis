/**
 * @file test_services.c
 * @brief Modulo di Test per i Servizi
 * @author Roberto Massante e Martina Gisonda
 */

#include <stdio.h>
#include <string.h>
#include "unity.h"
#include "../include/app_context.h"
#include "../include/config.h"
#include "../include/models/user.h"
#include "../include/models/match.h"
#include "../include/models/tennis_player.h"
#include "../include/services/auth_service.h"
#include "../include/services/players_service.h"
#include "../include/services/matches_service.h"
#include "services/scoring_service.h"
#include "test_services_data.h"
#include "services/teams_service.h"
#include "services/tournaments_service.h"

static AppContext app_context;
static char bad_file[50];

// Aggiunge dati al sistema che verranno usati dagli unit test
static void seed_test_data() {
    int players_count = sizeof(test_players) / sizeof(test_players[0]);
    for (int i = 0; i < players_count; i++) {
        entity_table_add_item(&app_context.tennisplayers_table, &test_players[i]);
    }

    int tournaments_count = sizeof(test_tournaments) / sizeof(test_tournaments[0]);
    for (int i = 0; i < tournaments_count; i++) {
        entity_table_add_item(&app_context.tournaments_table, &test_tournaments[i]);
    }

    int participations_count = sizeof(test_participations) / sizeof(test_participations[0]);
    for (int i = 0; i < participations_count; i++) {
        entity_table_add_item(&app_context.participations_table, &test_participations[i]);
    }

    int matches_count = sizeof(test_matches) / sizeof(test_matches[0]);
    for (int i = 0; i < matches_count; i++) {
        entity_table_add_item(&app_context.matches_table, &test_matches[i]);
    }

    int users_count = sizeof(test_users) / sizeof(test_users[0]);
    for (int i = 0; i < users_count; i++) {
        entity_table_add_item(&app_context.users_table, &test_users[i]);
    }

    int teams_count = sizeof(test_teams) / sizeof(test_teams[0]);
    for (int i = 0; i < teams_count; i++) {
        entity_table_add_item(&app_context.teams_table, &test_teams[i]);
    }
}

// Apre una sessione da amministratore
static void admin_login() {
    try_login(&app_context, DEFAULT_ADMIN_USERNAME, DEFAULT_ADMIN_PASSWORD);
}

// Apre una sessione da utente regolare
static void user_login() {
    try_login(&app_context, test_users[1].username, test_users[1].password);
}

// Eseguito prima di ogni unit test
void setUp() {
    // La lettura deve avvenire con successo
    TEST_ASSERT_EQUAL(READFILE_SUCCESS, init_app_context(&app_context, bad_file));

    // Previene qualsiasi scrittura su file
    entity_table_batch_begin(&app_context.matches_table);
    entity_table_batch_begin(&app_context.teams_table);
    entity_table_batch_begin(&app_context.participations_table);
    entity_table_batch_begin(&app_context.tennisplayers_table);
    entity_table_batch_begin(&app_context.users_table);
    entity_table_batch_begin(&app_context.tournaments_table);

    // Aggiungiamo dati con cui lavorare
    seed_test_data();
}

// Eseguito dopo ogni unit test
void tearDown() {
    // Liberiamo tutta la memoria dinamica per prevenire memory leak. Ripartiremo dallo stato iniziale con ogni test
    destroy_app_context(&app_context);
}

// Test n°1: Verifichiamo che la Registrazione di un profilo funzioni correttamente. Controlliamo che
// criteri sulla lunghezza della password, username unico vengano rispettati
static void test_register() {
    RegisterResult result = try_register(&app_context, "alice_123", "");
    // Blocchiamo la registrazione perchè la password è troppo corta?
    TEST_ASSERT_EQUAL(REGISTER_PASSWORD_TOO_SHORT, result);

    result = try_register(&app_context, "alice_123", "1234");
    // La registrazione avviene con successo?
    TEST_ASSERT_EQUAL(REGISTER_SUCCESS, result);

    result = try_register(&app_context, "alice_123", "1234");
    // Blocchiamo la registrazione perchè l'username esiste già?
    TEST_ASSERT_EQUAL(REGISTER_USERNAME_ALREADY_TAKEN, result);

    // Il nuovo utente non è un admin, vero?
    TEST_ASSERT_FALSE(app_context.is_admin);

    User user;

    // L'utente è presente nella tabella degli utenti?
    TEST_ASSERT_TRUE(get_user_by_id(&app_context, app_context.current_user_id, &user));

    // I parametri sono corretti?
    TEST_ASSERT_EQUAL_STRING("alice_123", user.username);
    TEST_ASSERT_FALSE(user.is_admin);

    logout(&app_context);
}

// Test n°2: Verifichiamo che il login funzioni correttamente
static void test_login() {
    LoginResult result = try_login(&app_context, DEFAULT_ADMIN_USERNAME, DEFAULT_ADMIN_PASSWORD);
    // Il Login è avvenuto con successo?
    TEST_ASSERT_EQUAL(result, LOGIN_SUCCESS);

    // L'ID dell'Utente è stato impostato nel contesto?
    TEST_ASSERT_EQUAL(1, app_context.current_user_id);

    // La flag di amministratore è stata impostata?
    TEST_ASSERT_TRUE(app_context.is_admin);
}

// Test n° 3: Verifichiamo che aggiungere e modificare Tennisti funzioni correttamente, con l'applicazione dei vari criteri.
static void test_player_add_edit() {
    admin_login();

    TennisPlayer player = { 0 };
    player.ranking = 1;
    TEST_ASSERT_EQUAL(STATUS_ERR_NAME_EMPTY, add_tennis_player(&app_context, &player, NULL));

    strcpy(player.name, "Aldo");
    TEST_ASSERT_EQUAL(STATUS_ERR_SURNAME_EMPTY, add_tennis_player(&app_context, &player, NULL));

    strcpy(player.surname, "Moro");
    player.ranking = 1;

    TEST_ASSERT_EQUAL(STATUS_OK, add_tennis_player(&app_context, &player, NULL));

    // Funziona il getter?
    TEST_ASSERT_TRUE(get_player_by_id(&app_context, player.id, &player));

    // Modificare parametri funziona? Persiste?
    player.ranking = 99;
    TEST_ASSERT_EQUAL(STATUS_OK, edit_tennis_player(&app_context, &player));

    TEST_ASSERT_TRUE(get_player_by_id(&app_context, player.id, &player));

    TEST_ASSERT_EQUAL(99, player.ranking);
}

// Test n° 4: Verifichiamo che i punteggi vengano calcolati correttamente.
static void test_scores(void) {
    Match match = { 0 };
    match.player_results[0].player_id = 1;
    match.player_results[0].won_sets = 2;
    match.player_results[0].aces = 5;
    match.player_results[0].breaks = 3;
    match.player_results[0].double_faults = 1;

    match.player_results[1].player_id = 2;
    match.player_results[1].won_sets = 0;
    match.player_results[1].aces = 2;
    match.player_results[1].breaks = 0;
    match.player_results[1].double_faults = 2;

    // Vincitore: +10 (vittoria) + 3 (vittoria in 2 set) + 2 (5 ace) + 1.5 (3 break) - 1 (doppi falli) = 15.5
    TEST_ASSERT_EQUAL_DOUBLE(15.5, get_match_score(&match, 0));

    // Perdente: -3 (sconfitta) + 0 (2 ace) + 0 (0 break) - 2 (doppi falli) = -5.0
    TEST_ASSERT_EQUAL_DOUBLE(-5.0, get_match_score(&match, 1));
}

// Test n° 5: Verifichiamo che l'impostazione di un Squadra funzioni correttamente, applicando i criteri di budget massimo,
// esistenza dei dati...
static void test_setteam() {
    user_login();

    uint32_t incomplete_players_ids[6] = {1, 2, 3, 0, 0, 0};
    uint32_t bad_players_ids[6] = {1, 2, 3, 4, 5, 6};
    uint32_t valid_players_ids[6] = {5, 6, 7, 8, 9, 10};

    ServiceStatus result = sign_up_tournament(&app_context, 2);
    TEST_ASSERT_EQUAL(STATUS_OK, result);

    // Torneo non esistente
    result = set_team(&app_context, 999, valid_players_ids);
    TEST_ASSERT_EQUAL(STATUS_ERR_USER_NOT_SIGNED_UP, result);

    // Torneo non aperto ad iscrizioni
    result = sign_up_tournament(&app_context, 1);
    TEST_ASSERT_EQUAL(STATUS_ERR_TOURNAMENT_NOT_OPEN, result);

    // Squadra non riempita
    result = set_team(&app_context, 2, incomplete_players_ids);
    TEST_ASSERT_EQUAL(STATUS_ERR_TEAM_NOT_FILLED, result);

    // Budget superato
    result = set_team(&app_context, 2, bad_players_ids);
    TEST_ASSERT_EQUAL(STATUS_ERR_BUDGET_EXCEEDED, result);

    // Tutto ok!
    result = set_team(&app_context, 2, valid_players_ids);
    TEST_ASSERT_EQUAL(STATUS_OK, result);
}

// Test n° 6: Verifichiamo che aggiungere una partita funzioni correttamente, verificando che parametri invalidi
// vengano bloccati
static void test_addmatch() {
    admin_login();

    Match match = { 0 };
    match.tournament_id = 1;
    match.player_results[0].player_id = 1;
    match.player_results[1].player_id = 1;

    // Blocchiamo partite con un giocatore contro se stesso?
    ServiceStatus result = add_match(&app_context, &match);
    TEST_ASSERT_EQUAL(STATUS_ERR_SAME_PLAYERS, result);

    match.player_results[1].player_id = 2;
    match.player_results[0].won_sets = 1;
    match.player_results[1].won_sets = 1;
    // Blocchiamo i pareggi?
    result = add_match(&app_context, &match);
    TEST_ASSERT_EQUAL(STATUS_ERR_TIE_NOT_ALLOWED, result);

    match.player_results[1].won_sets = 2;
    // Blocchiamo l'inserimento di partite ad un torneo non in corso?
    result = add_match(&app_context, &match);
    TEST_ASSERT_EQUAL(STATUS_ERR_TOURNAMENT_NOT_ONGOING, result);

    match.player_results[1].player_id = 9999;
    match.tournament_id = 2;

    // Impostiamo il torneo ad In Corso così che effettivamente possiamo inserire partite
    Tournament tournament = { 0 };
    get_tournament_by_id(&app_context, 2, &tournament);
    TEST_ASSERT_NOT_EQUAL(tournament.id, 0);

    tournament.state = TOURNAMENT_STATE_ONGOING;
    result = edit_tournament(&app_context, &tournament);
    TEST_ASSERT_EQUAL(result, STATUS_OK);

    // Blocchiamo l'inserimento di un tennista non valido?
    result = add_match(&app_context, &match);
    TEST_ASSERT_EQUAL(STATUS_ERR_INVALID_PLAYERS, result);

    // Tutto ok!
    match.player_results[1].player_id = 2;
    result = add_match(&app_context, &match);
    TEST_ASSERT_EQUAL(STATUS_OK, result);
}

// Test n° 7: Verifichiamo che la modifica dei dati di un Tennista sia bloccata se è impegnato in tornei in corso
static void test_playerbusy() {
    admin_login();

    // Impostiamo il torneo terminato su In Corso
    Tournament tournament = { 0 };
    get_tournament_by_id(&app_context, 1, &tournament);
    TEST_ASSERT_NOT_EQUAL(tournament.id, 0);

    tournament.state = TOURNAMENT_STATE_ONGOING;
    ServiceStatus result = edit_tournament(&app_context, &tournament);
    TEST_ASSERT_EQUAL(result, STATUS_OK);

    TennisPlayer player = { 0 };
    get_player_by_id(&app_context, 1, &player);
    TEST_ASSERT_NOT_EQUAL(player.id, 0);

    // Modifichiamo il cognome di Sinner a El Barese
    strcpy(player.surname, "El Barese");

    // Veniamo bloccati perchè il Tennista è impegnato in un torneo in corso?
    result = edit_tennis_player(&app_context, &player);
    TEST_ASSERT_EQUAL(STATUS_ERR_PLAYER_BUSY, result);

    // Reimpostiamo il torneo a Terminato
    tournament.state = TOURNAMENT_STATE_COMPLETED;
    tournament.winner_id = 1;
    result = edit_tournament(&app_context, &tournament);
    TEST_ASSERT_EQUAL(result, STATUS_OK);

    // Ora la modifica dovrebbe essere ammessa!
    result = edit_tennis_player(&app_context, &player);
    TEST_ASSERT_EQUAL(STATUS_OK, result);

    // Assicuriamoci che la modifica sia rimasta
    get_player_by_id(&app_context, 1, &player);

    TEST_ASSERT_EQUAL(strcmp(player.surname, "El Barese"), 0);
}

// Test n° 8: Verifichiamo che utenti normali siano respinti da servizi di amministrazione
static void test_admin() {
    user_login();
    TEST_ASSERT_EQUAL(STATUS_ERR_UNAUTHORIZED, add_tennis_player(&app_context, NULL, NULL));
    TEST_ASSERT_EQUAL(STATUS_ERR_UNAUTHORIZED, edit_tennis_player(&app_context, NULL));
    TEST_ASSERT_EQUAL(STATUS_ERR_UNAUTHORIZED, delete_tennis_player(&app_context, 0));
    TEST_ASSERT_EQUAL(STATUS_ERR_UNAUTHORIZED, add_match(&app_context, NULL));
    TEST_ASSERT_EQUAL(STATUS_ERR_UNAUTHORIZED, edit_match(&app_context, NULL));
    TEST_ASSERT_EQUAL(STATUS_ERR_UNAUTHORIZED, delete_match(&app_context, 0));
    TEST_ASSERT_EQUAL(STATUS_ERR_UNAUTHORIZED, add_tournament(&app_context, NULL, NULL));
    TEST_ASSERT_EQUAL(STATUS_ERR_UNAUTHORIZED, edit_tournament(&app_context, NULL));
    TEST_ASSERT_EQUAL(STATUS_ERR_UNAUTHORIZED, delete_tournament(&app_context, 0));
    TEST_ASSERT_EQUAL(STATUS_ERR_UNAUTHORIZED, add_tournament_participation(&app_context, 0, 0));
    TEST_ASSERT_EQUAL(STATUS_ERR_UNAUTHORIZED, delete_tournament_participation(&app_context, 0, 0));
}

// Test n° 9: Verifichiamo che la classifica dei Tennisti venga calcolata correttamente comparando i valori calcolati
// dalla funzione con valori calcolati a mano
static void test_leaderboards() {
    admin_login();

    size_t scores_count;
    PlayerScore* players_board = get_global_players_leaderboard(&app_context, &scores_count);
    // Assicuriamoci che sia stata generata correttamente!
    TEST_ASSERT_NOT_NULL(players_board);

    // Calcolati manualmente, se non sono giusti, c'è qualcosa che non va
    PlayerScore expected_scores[10] = {
        { .tennisplayer_id = 5, .score = 13.5 },
        { .tennisplayer_id = 1, .score = 12 },
        { .tennisplayer_id = 7, .score = 12 },
        { .tennisplayer_id = 3, .score = 6.5 },
        { .tennisplayer_id = 8, .score = 0 },
        { .tennisplayer_id = 9, .score = 0 },
        { .tennisplayer_id = 10, .score = 0 },
        { .tennisplayer_id = 2, .score = -3.5 },
        { .tennisplayer_id = 4, .score = -5 },
        { .tennisplayer_id = 6, .score = -6 },
    };

    for (int i = 0; i < scores_count; i++) {
        TEST_ASSERT_EQUAL(expected_scores[i].tennisplayer_id, players_board[i].tennisplayer_id);
        TEST_ASSERT_EQUAL(expected_scores[i].score, players_board[i].score);
    }
}

// Test n° 10: verifichiamo che tutto il ciclo di vita di un Torneo venga gestito correttamente, dallo stato di Aperto
// allo stato di Completato, verificando che tutti i criteri ncessari per ogni transizione vengano sempre rispettati
static void test_tournament_lifecycle() {
    admin_login();

    uint32_t tournament_id;
    Tournament tournament = { 0 };

    // Blocchiamo la creazione di tornei con titoli vuoti?
    ServiceStatus status = add_tournament(&app_context, &tournament, &tournament_id);
    TEST_ASSERT_EQUAL(STATUS_ERR_TITLE_EMPTY, status);

    // Ora creiamolo davvero
    strcpy(tournament.title, "Torneo dei Test");
    status = add_tournament(&app_context, &tournament, &tournament_id);
    TEST_ASSERT_EQUAL(STATUS_OK, status);

    // Non può iniziare fino a quando non vengono impostati i partecipanti!
    tournament.state = TOURNAMENT_STATE_ONGOING;
    status = edit_tournament(&app_context, &tournament);
    TEST_ASSERT_EQUAL(STATUS_ERR_NOT_ENOUGH_PLAYERS, status);

    // Assicuriamoci anche che terminarlo subito sia bloccato
    tournament.state = TOURNAMENT_STATE_COMPLETED;
    status = edit_tournament(&app_context, &tournament);
    TEST_ASSERT_EQUAL(STATUS_ERR_ILLEGAL_TRANSITION, status);

    for (int i = 1; i < 10; i++) {
        status = add_tournament_participation(&app_context, i, tournament_id);
        TEST_ASSERT_EQUAL(STATUS_OK, status);
    }

    // Assicuriamoci che Tennisti non disponibili non possono essere aggiunti
    // Impostiamo l'ultimo come non disponibile
    TennisPlayer unavailable_player;
    TEST_ASSERT_TRUE(get_player_by_id(&app_context, 10, &unavailable_player));
    unavailable_player.is_available = false;
    status = edit_tennis_player(&app_context, &unavailable_player);
    TEST_ASSERT_EQUAL(STATUS_OK, status);

    // e proviamo
    status = add_tournament_participation(&app_context, 10, tournament_id);
    TEST_ASSERT_EQUAL(STATUS_ERR_PLAYER_UNAVAILABLE, status);

    // Ora abbiamo abbastanza Tennisti per far iniziare il torneo
    tournament.state = TOURNAMENT_STATE_ONGOING;
    status = edit_tournament(&app_context, &tournament);
    TEST_ASSERT_EQUAL(STATUS_OK, status);

    // Non si può concludere fino a quando non è stata giocata almeno una partita
    tournament.state = TOURNAMENT_STATE_COMPLETED;
    status = edit_tournament(&app_context, &tournament);
    TEST_ASSERT_EQUAL(STATUS_ERR_TOURNAMENT_NOT_STARTED, status);

    // Aggiungiamo una partita
    Match new_match = {
        .tournament_id = tournament_id,
        .player_results = {
                    { .player_id = 1, .won_sets = 3, .aces = 12, .double_faults = 2, .breaks = 4 },
                    { .player_id = 2, .won_sets = 1, .aces = 7,  .double_faults = 3, .breaks = 1 }
        }
    };

    status = add_match(&app_context, &new_match);
    TEST_ASSERT_EQUAL(STATUS_OK, status);

    // Ora non possiamo più far ritornare il torneo ad aperto
    tournament.state = TOURNAMENT_STATE_OPEN;
    status = edit_tournament(&app_context, &tournament);
    TEST_ASSERT_EQUAL(STATUS_ERR_TOURNAMENT_ALREADY_STARTED, status);

    // Ma non possiamo neanche impostarlo come terminato fino a quando non impostiamo un vincitore
    tournament.state = TOURNAMENT_STATE_COMPLETED;
    status = edit_tournament(&app_context, &tournament);
    TEST_ASSERT_EQUAL(STATUS_ERR_WINNER_NOT_SET, status);

    // Quindi ne impostiamo uno...
    tournament.winner_id = 1;
    status = edit_tournament(&app_context, &tournament);
    TEST_ASSERT_EQUAL(STATUS_OK, status);
    // e dovremmo esserci

    // Ovviamente, non possiamo impostarlo ad aperto arrivati a questo punto
    tournament.state = TOURNAMENT_STATE_OPEN;
    status = edit_tournament(&app_context, &tournament);
    TEST_ASSERT_EQUAL(STATUS_ERR_ILLEGAL_TRANSITION, status);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_register);
    RUN_TEST(test_login);
    RUN_TEST(test_player_add_edit);
    RUN_TEST(test_scores);
    RUN_TEST(test_setteam);
    RUN_TEST(test_addmatch);
    RUN_TEST(test_playerbusy);
    RUN_TEST(test_admin);
    RUN_TEST(test_leaderboards);
    RUN_TEST(test_tournament_lifecycle);

    return UNITY_END();
}
