/**
 * @file players_service.h
 * @brief Modulo dei servizi posti a disposizione per interagire con i dati sui Tennisti
 * @author Roberto Massante e Martina Gisonda
*/

#ifndef FANTATENNIS_PLAYERS_SERVICE_H
#define FANTATENNIS_PLAYERS_SERVICE_H

#include "../models/tennis_player.h"
#include "../app_context.h"
#include "../common_types.h"
#include <stdlib.h>

/**
 * @brief Struttura usata per raccogliere i possibili filtri di ricerca sui Tennisti
 */
typedef struct {
    char name_filter[TENNISPLAYER_NAME_SIZE];       /**< Il filtro per il Nome del Tennista. Se vuoto, non verrà applicato */
    char surname_filter[TENNISPLAYER_SURNAME_SIZE]; /**< Il filtro per il Cognome del Tennista. Se vuoto, non verrà applicato */
    FilterBool availability_filter;                 /**< Il filtro per la disponibilità del Tennista. Se impostato a FILTER_NOT_SET, non verrà applicato */
} PlayerSearchFilter;

/**
 * Funzione di Servizio per ottenere una lista di copie di Tennisti nel sistema in base ai criteri di ricerca passati.
 * @warning L'array di tennisti restituito è allocato dinamicamente a causa della quantità variabile di risultati -
 * è OBBLIGATORIO rilasciare la memoria usando free() sul valore restituito da questo metodo per prevenire un memory leak!
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param filter La raccolta di filtri da applicare alla ricerca. Se NULL, non verrà applicato alcun filtro.
 * @param out_count Il puntatore alla variabile che conterrà il numero di Tennisti restituiti dalla funzione.
 * Sarà impostata a SIZE_MAX se non c'è abbastanza memoria nel sistema per completare l'operazione
 * @return Un array delle copie dei Tennisti che rientrano nei criteri di ricerca. NULL se non ci sono elementi da restituire.
 */
TennisPlayer* get_tennis_players(const AppContext* app_context, const PlayerSearchFilter* filter, size_t* out_count);

/**
 * Funzione di Servizio per ottenere il costo di un Tennista in crediti.
 * @details Il costo viene calcolato in base al Ranking del Tennista, e anche in base al Ranking di tutti gli altri Tennisti nel Torneo specificato.
 * Di conseguenza, il costo non è fisso, ma dipende anche dal Torneo specifico, e varia in base a chi ci partecipa. Sebbene questo non permetta
 * funzioni come un filtro di ricerca in base al costo, permette un miglior bilanciamento dei costi, soprattutto per Tornei dove per caso partecipano
 * solo Tennisti di rank altissimi, o di rank bassissimi
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tournament_id Il torneo per il quale si sta calcolando il costo del Tennista
 * @param tennisplayer_id Il tennista di cui calcolare il costo
 * @return Il costo del Tennista in crediti. Sarà 0 se il torneo o il tennista specificati sono invalidi, altrimenti un numero tra MIN_PLAYER_COST e MAX_PLAYER_COST
 */
unsigned int get_player_cost(const AppContext* app_context, uint32_t tournament_id, uint32_t tennisplayer_id);

/**
 * Funzione di Servizio per aggiungere un Tennista al sistema.
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param new_player Il puntatore allo struct del nuovo Tennista da aggiungere. I dati lì presenti saranno clonati ed inseriti nella tabella
 * @param out_id Il puntatore ad una variabile size_t che conterrà l'ID del Tennista appena inserito
 * @return L'esito dell'operazione (ServiceStatus).
 * - STATUS_ERR_UNAUTHORIZED se l'utente non ha i privilegi necessari per eseguire questa operazione
 * - STATUS_ERR_NAME_EMPTY se il nome è vuoto (non permesso)
 * - STATUS_ERR_SURNAME_EMPTY se il cognome è vuoto (non permesso)
 * - STATUS_ERR_RANKING_NOT_SET se il ranking non è impostato (non permesso)
 * - STATUS_ERR_OOM se il sistema ha esaurito la memoria e non può aggiungere ulteriori dati
 * - STATUS_OK se l'operazione è avvenuta con successo
 */
ServiceStatus add_tennis_player(AppContext* app_context, TennisPlayer* new_player, uint32_t* out_id);

/**
 * Funzione di Servizio per modificare un Tennista nel sistema.
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param edited_player Il puntatore allo struct del Tennista con i dati modificati. I dati lì presenti saranno clonati ed inseriti nella tabella
 * @return L'esito dell'operazione (ServiceStatus)
 * - STATUS_ERR_UNAUTHORIZED se l'utente non ha i privilegi necessari per eseguire questa operazione
 * - STATUS_ERR_NAME_EMPTY se il nome è vuoto (non permesso)
 * - STATUS_ERR_SURNAME_EMPTY se il cognome è vuoto (non permesso)
 * - STATUS_ERR_RANKING_NOT_SET se il ranking non è impostato (non permesso)
 * - STATUS_ERR_PLAYER_BUSY se il tennista è attualmente coinvolto in uno o più tornei in corso, in tal caso la modifica dei suoi dati non è permessa
 * - STATUS_ERR_NOT_FOUND se l'ID nella struttura non corrisponde ad alcun tennista attualmente nel sistema
 * - STATUS_OK se l'operazione è avvenuta con successo
 */
ServiceStatus edit_tennis_player(AppContext* app_context, TennisPlayer* edited_player);

/**
 * Funzione di Servizio per eliminare un Tennista nel sistema.
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param id L'ID del Tennista da eliminare
 * @return L'esito dell'operazione (ServiceStatus)
 * - STATUS_ERR_UNAUTHORIZED se l'utente non ha i privilegi necessari per eseguire questa operazione
 * - STATUS_ERR_PLAYER_HAS_HISTORY se il tennista è coinvolto in uno o più tornei nel sistema; eliminarlo/a altererebbe e rovinerebbe i risultati del torneo,
 * quindi la cancellazione di tennisti è consentita solo se non sono partecipanti in alcun torneo.
 * - STATUS_ERR_NOT_FOUND se l'ID nella struttura non corrisponde a nessun tennista attualmente nel sistema
 * - STATUS_OK se l'operazione è avvenuta con successo
 */
ServiceStatus delete_tennis_player(AppContext* app_context, uint32_t id);

/**
 * Funzione di Servizio che restituisce una copia del Tennista il cui ID corrisponde a quello specificato.
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param id L'ID del Tennista da ricercare
 * @param player Il puntatore alla variabile nella quale verranno copiati i dettagli del Tennista ricercato
 * @return true se la ricerca è avvenuta con successo, false non è stato trovato alcun giocatore con l'ID richiesto
 */
bool get_player_by_id(const AppContext* app_context, uint32_t id, TennisPlayer* player);

#endif
