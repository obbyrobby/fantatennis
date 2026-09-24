/**
 * @file cli_mainmenu.h
 * @brief Modulo di CLI per il menu principale e opzioni secondarie
 * @author Roberto Massante e Martina Gisonda
*/

#ifndef FANTATENNIS_CLI_MAINMENU_H
#define FANTATENNIS_CLI_MAINMENU_H

#include "../app_context.h"

/**
 * Funzione di CLI che apre il menu principale per utenti regolari
 * @param app_context Il puntatore al contesto dell'applicazione
 */
void cli_regularmenu(AppContext* app_context);

/**
 * Funzione di CLI che apre il menu principale per utenti amministratori
 * @param app_context Il puntatore al contesto dell'applicazione
 */
void cli_adminmenu(AppContext* app_context);

#endif
