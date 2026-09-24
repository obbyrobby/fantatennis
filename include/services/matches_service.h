/**
 * @file matches_service.h
 * @brief Modulo dei servizi posti a disposizione per interagire con i dati sulle Partite
 * @author Roberto Massante e Martina Gisonda
*/

#ifndef FANTATENNIS_MATCHES_SERVICE_H
#define FANTATENNIS_MATCHES_SERVICE_H

#include "../app_context.h"
#include "../models/match.h"
#include "../common_types.h"

/**
 * Funzione di Servizio per ottenere una lista di copie di partite di un Torneo.
 * @warning L'array restituito è allocato dinamicamente a causa della quantità variabile di risultati -
 * è OBBLIGATORIO rilasciare la memoria usando free() sul valore restituito da questo metodo per prevenire un memory leak!
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tournament_id L'ID del Torneo del quale ottenere le partite
 * @param matches_count Il puntatore alla variabile che conterrà il numero di partite.
 * Sarà impostata a SIZE_MAX se il sistema non ha memoria sufficiente per eseguire l'operazione (OOM).
 * @return Un array delle copie delle partite del torneo specificato. NULL se non ci sono elementi da restituire.
 */
Match* get_tournament_matches(const AppContext* app_context, uint32_t tournament_id, size_t* matches_count);

/**
 * Funzione di Servizio per ottenere la quantità di partite giocate in un Torneo.
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tournament_id L'ID del Torneo nel quale contare le partite
 * @return La quantità di partite giocate nel Torneo specificato.
 */
size_t get_tournament_matches_quantity(const AppContext* app_context, uint32_t tournament_id);

/**
 * Funzione di Servizio per aggiungere una nuova Partita al sistema.
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param new_match Il puntatore allo struct della nuova Partita da aggiungere. I dati lì presenti saranno clonati ed inseriti nella tabella
 * @return L'esito dell'operazione (ServiceStatus).
 * - STATUS_ERR_UNAUTHORIZED se l'utente non ha i privilegi necessari per questa operazione
 * - STATUS_ERR_SAME_PLAYERS se i due sfidanti sono uguali
 * - STATUS_ERR_TIE_NOT_ALLOWED se il numero di set vinti tra gli sfidanti è uguale: il pareggio non esiste nel Tennis
 * - STATUS_ERR_TOURNAMENT_NOT_FOUND se il torneo specificato nell'oggetto non esiste
 * - STATUS_ERR_TOURNAMENT_NOT_ONGOING se il torneo specificato nell'oggetto non è in corso; si possono aggiungere partite solo se lo è
 * - STATUS_ERR_INVALID_PLAYERS se i giocatori specificati nell'oggetto non sono partecipanti del torneo specificato
 * - STATUS_ERR_OOM se il sistema ha esaurito la memoria
 * - STATUS_OK se l'operazione è avvenuta con successo
 */
ServiceStatus add_match(AppContext* app_context, Match* new_match);

/**
 * Funzione di Servizio per modificare una Partita nel sistema.
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param edited_match Il puntatore allo struct della Partita con i dati modificati. I dati lì presenti saranno clonati ed inseriti nella tabella
 * @return L'esito dell'operazione (ServiceStatus).
 * - STATUS_ERR_UNAUTHORIZED se l'utente non ha i privilegi necessari per questa operazione
 * - STATUS_ERR_SAME_PLAYERS se i due sfidanti sono uguali
 * - STATUS_ERR_TIE_NOT_ALLOWED se il numero di set vinti tra gli sfidanti è uguale: il pareggio non esiste nel Tennis
 * - STATUS_ERR_TOURNAMENT_NOT_FOUND se il torneo specificato nell'oggetto non esiste
 * - STATUS_ERR_TOURNAMENT_NOT_ONGOING se il torneo specificato nell'oggetto non è in corso; si possono modificare partite solo se lo è
 * - STATUS_ERR_INVALID_PLAYERS se i giocatori specificati nell'oggetto non sono partecipanti del torneo specificato
 * - STATUS_ERR_NOT_FOUND se l'ID nell'oggetto della Partita non esiste nel sistema
 * - STATUS_OK se l'operazione è avvenuta con successo
 *
 */
ServiceStatus edit_match(AppContext* app_context, Match* edited_match);

/**
 * Funzione di Servizio per eliminare una Partita nel sistema
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param id L'ID della Partita da eliminare
 * @return L'esito dell'operazione (ServiceStatus).
 * - STATUS_ERR_UNAUTHORIZED se l'utente non ha i privilegi necessari per questa operazione
 * - STATUS_ERR_NOT_FOUND se l'ID nell'oggetto della Partita non esiste nel sistema
 * - STATUS_ERR_TOURNAMENT_NOT_ONGOING se il torneo specificato nell'oggetto non è in corso; si possono eliminare partite solo se lo è
 * - STATUS_OK se l'operazione è avvenuta con successo
 */
ServiceStatus delete_match(AppContext* app_context, uint32_t id);

/**
 * Funzione di Servizio che restituisce una copia della Partita il cui ID corrisponde a quello specificato
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param id L'ID della Partita da cercare
 * @param match Il puntatore alla variabile nella quale verranno copiati i dettagli della partita
 * @return true se la ricerca è avvenuta con successo, false se non è stato trovato alcuna partita con l'ID richiesto
 */
bool get_match_by_id(const AppContext* app_context, uint32_t id, Match* match);

#endif
