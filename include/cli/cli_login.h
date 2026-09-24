/**
 * @file cli_login.h
 * @brief Modulo di CLI per le interazioni di login e registrazione al sistema
 * @author Roberto Massante e Martina Gisonda
*/

#ifndef FANTATENNIS_CLI_LOGIN_H
#define FANTATENNIS_CLI_LOGIN_H

#include "../app_context.h"

/**
 * Funzione di CLI che permette all'utente di fare login o registrarsi.
 * @param app_context Il puntatore al contesto dell'applicazione
 * @return false se l'utente ha annullato l'operazione, true se ha effettuato il login
 */
bool cli_login(AppContext* app_context);

#endif
