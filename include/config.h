/**
 * @file config.h
 * @brief Modulo di configurazione del sistema, contenente svariate costanti vitali al funzionamento del programma
 * @author Roberto Massante e Martina Gisonda
 */

#ifndef FANTATENNIS_CONFIG_H
#define FANTATENNIS_CONFIG_H

// Lunghezze delle stringhe
// Tutte le lunghezze di stringhe sotto definite tengono in considerazione la presenza del NULL terminator alla fine
#define USERNAME_SIZE 21
#define PASSWORD_SIZE 21

#define MIN_USERNAME_LEN 2
#define MIN_PASSWORD_LEN 3

#define TENNISPLAYER_NAME_SIZE 31
#define TENNISPLAYER_SURNAME_SIZE 31

// Costo massimo e minimo in crediti per ogni tennista
#define MAX_PLAYER_COST 35
#define MIN_PLAYER_COST 8

// Numero di giocatori in una squadra
#define TEAM_SIZE 6

// Max budget per una squadra
#define MAX_TEAM_BUDGET 100

// Nomi dei file di storage
#ifndef UNIT_TESTS
    #define USERS_FILENAME "utenti.bin"
    #define TENNISPLAYERS_FILENAME "tennisti.bin"
    #define TOURNAMENTS_FILENAME "tornei.bin"
    #define MATCHES_FILENAME "partite.bin"
    #define TEAMS_FILENAME "squadre.bin"
    #define PARTICIPATIONS_FILENAME "partecipazioni.bin"
#else
    #define USERS_FILENAME "utenti_test.bin"
    #define TENNISPLAYERS_FILENAME "tennisti_test.bin"
    #define TOURNAMENTS_FILENAME "tornei_test.bin"
    #define MATCHES_FILENAME "partite_test.bin"
    #define TEAMS_FILENAME "squadre_test.bin"
    #define PARTICIPATIONS_FILENAME "partecipazioni_test.bin"
#endif


// Magic numbers per i file di storage
#define USERS_MAGICNUMBER 0x4369616F5574656ELL // CiaoUten
#define TENNISPLAYERS_MAGICNUMBER 0x486F6C61546E7374LL // HolaTnst
#define TOURNAMENTS_MAGICNUMBER 0x4747546F726E656F // GGTorneo
#define MATCHES_MAGICNUMBER 0x4C65506172746974 // LePartit
#define TEAMS_MAGICNUMBER 0x5371756164732121 // Squads!!
#define PARTICIPATIONS_MAGICNUMBER 0x7061727465636970 // partecip

// Limiti di capienza degli array dinamici
#define MAX_USERS 10000
#define MAX_MATCHES 100000
#define MAX_TENNISPLAYERS 10000
#define MAX_TOURNAMENTS 10000
#define MAX_TEAMS 100000
#define MAX_PARTICIPATIONS (MAX_TENNISPLAYERS * MAX_TOURNAMENTS)

// Credenziali dell'admin di default
#define DEFAULT_ADMIN_USERNAME "admin"
#define DEFAULT_ADMIN_PASSWORD "uniba"

#endif
