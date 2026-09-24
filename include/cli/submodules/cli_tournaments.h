/**
 * @file cli_tournaments.h
 * @brief Modulo di CLI per le interazioni riguardanti i tornei
 * @author Roberto Massante e Martina Gisonda
*/

#ifndef FANTATENNIS_CLI_TOURNAMENTS_H
#define FANTATENNIS_CLI_TOURNAMENTS_H

#include "../../app_context.h"
#include "../../models/tournament.h"
#include "../../common_types.h"

/**
 * Funzione di CLI riutilizzabile per stampare su terminale tutti i dettagli di un singolo Torneo
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tournament Il puntatore al Torneo di cui mostrare dati e dettagli.
 */
void cli_print_single_tournament(const AppContext* app_context, Tournament* tournament);

/**
 * Funzione di CLI che mostra all'utente tutti i Tornei nel sistema
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param data Inutilizzato
 */
void cli_view_tournaments(AppContext* app_context, void* data);

/**
 * Funzione di CLI che permette all'utente di selezionare un Torneo tra quelli presentati.
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tournament Il puntatore alla variabile di tipo Tournament nella quale saranno copiati tutti i dati del Torneo selezionato
 * @param state_filter Il filtro per lo stato del Torneo
 * @return true se è stato selezionato un torneo, false se l'utente ha annullato la selezione
 */
bool cli_select_tournament(const AppContext* app_context, Tournament* tournament, FilterTournamentState state_filter);

/**
 * Funzione di CLI che mostra all'utente amministratore un menu per gestire i Tornei nel sistema (CRUD)
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param data Inutilizzato
 */
void cli_admin_menu_tournaments(AppContext* app_context, void* data);

#endif
