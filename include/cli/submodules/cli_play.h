/**
 * @file cli_play.h
 * @brief Modulo di CLI per le interazioni che consentono ad un Fanta-Coach di giocare ad un Torneo
 * @author Roberto Massante e Martina Gisonda
*/

#ifndef FANTATENNIS_CLI_PLAY_H
#define FANTATENNIS_CLI_PLAY_H
#include "../../app_context.h"

/**
 * Funzione di CLI che permette ad un Fanta-Coach di iscriversi ad un Torneo
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param data Inutilizzato
 */
void cli_sign_up_tournament(AppContext* app_context, void* data);

/**
 * Funzione di CLI che mostra al Fanta-Coach un menu con varie opzioni per interagire e partecipare al Torneo scelto
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param data Inutilizzato
 */
void cli_play_tournament(AppContext* app_context, void* data);

/**
 * Funzione di CLI che mostra all'utente la classifica dei Tennisti di un Torneo specifico
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tournament_ptr Il puntatore al Torneo per il quale generare la classifica
 */
void cli_view_players_leaderboard(AppContext* app_context, void* tournament_ptr);

/**
 * Funzione di CLI che mostra all'utente la classifica dei Fanta-Coach di un Torneo specifico
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tournament_ptr Il puntatore al Torneo per il quale generare la classifica
 */
void cli_view_users_leaderboard(AppContext* app_context, void* tournament_ptr);

#endif
