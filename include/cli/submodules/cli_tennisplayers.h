/**
 * @file cli_tennisplayers.h
 * @brief Modulo di CLI per le interazioni riguardanti i Tennisti
 * @author Roberto Massante e Martina Gisonda
*/

#ifndef FANTATENNIS_CLI_TENNISPLAYERS_H
#define FANTATENNIS_CLI_TENNISPLAYERS_H
#include "../../app_context.h"
#include "../../models/tennis_player.h"

/**
 * Funzione di CLI che mostra all'utente un menu' di ricerca per cercare Tennisti nel sistema in base a vari criteri
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param data Inutilizzato
 */
void cli_search_menu_tennisplayers(AppContext* app_context, void* data);

/**
 * Funzione di CLI che mostra all'utente amministratore un menu per gestire i Tennisti nel sistema (CRUD)
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param data Inutilizzato
 */
void cli_admin_menu_tennisplayers(AppContext* app_context, void* data);

/**
 * Funzione di CLI riutilizzabile per stampare su terminale una lista di Tennisti
 * @param players L'array dei Tennisti da stampare
 * @param count La quantità dei Tennisti da stampare
 * @param compact_view Opzione per visualizzazione compatta. Se è true, per ogni tennista verranno stampati solo ID, nome e cognome
 */
void cli_view_tennisplayers(TennisPlayer* players, size_t count, bool compact_view);

#endif
