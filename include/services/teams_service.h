/**
 * @file teams_service.h
 * @brief Modulo del servizio posto a disposizione per interagire con i dati sulle Squadre
 * @author Roberto Massante e Martina Gisonda
*/

#ifndef FANTATENNIS_TEAMS_SERVICE_H
#define FANTATENNIS_TEAMS_SERVICE_H

#include "../app_context.h"
#include "../models/team.h"
#include "../common_types.h"
#include "../config.h"
#include <stdlib.h>

/**
 * Funzione di Servizio per ottenere una lista di copie di Squadre registrate per un Torneo
 * @warning L'array di squadre restituito è allocato dinamicamente a causa della quantità variabile di risultati -
 * è OBBLIGATORIO rilasciare la memoria usando free() sul valore restituito da questo metodo per prevenire un memory leak!
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tournament_id L'ID del Torneo di interesse
 * @param teams_count Il puntatore alla variabile che conterrà il numero di Squadre restituite dalla funzione.
 * Sarà impostata a SIZE_MAX se non c'è abbastanza memoria nel sistema per completare l'operazione
 * @return Un array delle copie delle Squadre registrate per il Torneo specificato. NULL se non ci sono elementi da restituire.
 */
Team* get_tournament_teams(const AppContext* app_context, uint32_t tournament_id, size_t* teams_count);

/**
 * Funzione di Servizio per ottenere la quantità di Squadre registrate per un Torneo
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tournament_id L'ID del Torneo di interesse
 * @return La quantità di squadre registrate per il torneo specificato
 */
size_t get_tournament_teams_quantity(const AppContext* app_context, uint32_t tournament_id);

/**
 * Funzione di Servizio per ottenere tutte le squadre di un Utente/Fanta-Coach
 * @warning L'array di squadre restituito è allocato dinamicamente a causa della quantità variabile di risultati -
 * è OBBLIGATORIO rilasciare la memoria usando free() sul valore restituito da questo metodo per prevenire un memory leak!
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param user_id L'ID dell'utente interessato
 * @param teams_count Il puntatore alla variabile che conterrà il numero di Squadre restituite dalla funzione.
 * Sarà impostata a SIZE_MAX se non c'è abbastanza memoria nel sistema per completare l'operazione
 * @return Un array delle copie delle Squadre dell'Utente specificato. NULL se non ci sono elementi da restituire.
 */
Team* get_user_teams(const AppContext* app_context, uint32_t user_id, size_t* teams_count);

/**
 * Funzione di Servizio che restituisce una copia della squadra di un specifico utente, per uno specifico torneo
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tournament_id L'ID del Torneo per la quale è stata registrata questa squadra
 * @param user_id L'ID dell'Utente che ha registrato questa squadra
 * @param out_team Il puntatore alla variabile nella quale verranno copiati i dettagli della squadra
 * @return true se la ricerca è avvenuta con successo, false se non è stata trovata la squadra
 */
bool get_specific_team(const AppContext* app_context, uint32_t tournament_id, uint32_t user_id, Team* out_team);

/**
 * Funzione di Servizio che imposta i Tennisti di una Squadra
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tournament_id L'ID del torneo per il quale sta venendo impostata questa squadra
 * @param player_ids Gli ID dei tennisti scelti per la Squadra
 * @return L'esito dell'operazione (ServiceStatus).
 * - STATUS_ERR_USER_NOT_SIGNED_UP se l'Utente specificato non si è ancora iscritto al Torneo specificato (deve iscriversi prima di poter impostare una squadra)
 * - STATUS_ERR_TOURNAMENT_NOT_FOUND se l'ID del Torneo specificato non esiste
 * - STATUS_ERR_TOURNAMENT_NOT_OPEN se il Torneo specificato non è più aperto; in tal caso, iscrizioni e scelte dei Team non sono più accettate
 * - STATUS_ERR_OOM se il sistema ha terminato la memoria disponibile per completare l'operazione
 * - STATUS_ERR_PLAYER_NOT_IN_TOURNAMENT se uno o più Tennisti specificati in players_ids non è un partecipante del Torneo specificato
 * - STATUS_ERR_TEAM_NOT_FILLED se la Squadra non è stata riempita con tutti e 6 i Tennisti
 * - STATUS_ERR_PLAYER_DUPED se lo stesso Tennista appare due o più volte
 * - STATUS_ERR_PLAYER_UNAVAILABLE se è stato impostato un Tennista nel Team che non è attualmente disponibile
 * - STATUS_ERR_BUDGET_EXCEEDED se il costo in crediti del Team supera il budget massimo consentito
 * - STATUS_OK se l'operazione è stata eseguita con successo
 */
ServiceStatus set_team(AppContext* app_context, uint32_t tournament_id, const uint32_t player_ids[TEAM_SIZE]);

#endif
