/**
 * @file cli_matches.h
 * @brief Modulo di CLI per le interazioni riguardanti le partite
 * @author Roberto Massante e Martina Gisonda
*/

#ifndef FANTATENNIS_CLI_MATCHES_H
#define FANTATENNIS_CLI_MATCHES_H

#include "../../app_context.h"

/**
 * Funzione di CLI che mostra all'utente amministratore un menu per gestire le partite nel sistema (CRUD)
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param data inutilizzato
 */
void cli_admin_menu_matches(AppContext* app_context, void* data);

/**
 * Funzione di CLI che mostra all'utente le partite di un torneo specifico
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param data FACOLTATIVO - puntatore al Torneo del quale si desidera gestire le partite. Se NULL, la funzione
 * farà scegliere una partita all'utente autonomamente
 */
void cli_view_matches(AppContext* app_context, void* data);

#endif
