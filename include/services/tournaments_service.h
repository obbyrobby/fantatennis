/**
 * @file tournaments_service.h
 * @brief Modulo dei servizi posti a disposizione per interagire con i dati sui Tornei
 * @author Roberto Massante e Martina Gisonda
*/

#ifndef FANTATENNIS_TOURNAMENTS_SERVICE_H
#define FANTATENNIS_TOURNAMENTS_SERVICE_H

#include <stdlib.h>
#include "../app_context.h"
#include "../models/tournament.h"
#include "../common_types.h"
#include "../models/tennis_player.h"

/**
 * Funzione di Servizio per ottenere una lista di copie di tornei dove il tennista specificato è partecipante.
 * @warning L'array di tornei restituito è allocato dinamicamente a causa della quantità variabile di risultati -
 * è OBBLIGATORIO rilasciare la memoria usando free() sul valore restituito da questo metodo per prevenire un memory leak!
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tennisplayer_id L'ID del tennista di cui controllare le partecipazioni ad ogni torneo
 * @param participations_count Il puntatore alla variabile che conterrà il numero di tornei restituiti dalla funzione.
 * Sarà impostata a SIZE_MAX se non c'è abbastanza memoria nel sistema per completare l'operazione
 * @return Un array delle copie dei tornei che rientrano nei criterio di ricerca. NULL se non ci sono elementi da restituire.
 */
Tournament* get_player_tournaments(const AppContext* app_context, uint32_t tennisplayer_id, size_t* participations_count);

/**
 * Funzione di Servizio per ottenere la quantità di tornei ai quali il tennista specificato ha partecipato/partecipa.
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tennisplayer_id L'ID del tennista di cui controllare le partecipazioni ad ogni torneo
 * @return La quantità di tornei ai quali il giocatore specificato partecipa.
 */
size_t get_player_tournaments_quantity(const AppContext* app_context, uint32_t tennisplayer_id);

/**
 * Funzione di Servizio che restituisce una lista di copie di tornei attualmente in corso ai quali il tennista specificato sta partecipando.
 * @warning L'array di tornei restituito è allocato dinamicamente a causa della quantità variabile di risultati -
 * è OBBLIGATORIO rilasciare la memoria usando free() sul valore restituito da questo metodo per prevenire un memory leak!
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tennisplayer_id L'ID del tennista
 * @param tournaments_count Il puntatore alla variabile che conterrà il numero di tornei restituiti.
 * @return Un array delle copie dei tornei che rientrano nel criterio di ricerca. NULL se non ci sono elementi da restituire.
 */
Tournament* get_player_ongoing_tournaments(const AppContext* app_context, uint32_t tennisplayer_id, size_t* tournaments_count);

/**
 * Funzione di Servizio per ottenere la quantità di tornei attualmente in corso ai quali il tennista specificato sta partecipando.
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tennisplayer_id L'ID del tennista
 * @return La quantità dei tornei che rientrano nei criteri di ricerca.
 */
size_t get_player_ongoing_tournaments_quantity(const AppContext* app_context, uint32_t tennisplayer_id);

/**
 * Funzione di Servizio per ottenere una lista di copie di tornei.
 * @warning L'array restituito è allocato dinamicamente a causa della quantità variabile di risultati -
 * è OBBLIGATORIO rilasciare la memoria usando free() sul valore restituito da questo metodo per prevenire un memory leak!
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param state_filter Filtra i tornei in base allo stato. Per non filtrare, passare FILTER_TOURNAMENT_STATE_ALL.
 * @param tournaments_count Il puntatore alla variabile che conterrà il numero di tornei restituiti dalla funzione.
 * Sarà impostata a SIZE_MAX se non c'è abbastanza memoria nel sistema per completare l'operazione
 * @return Un array delle copie dei tornei che rientrano nei criteri specificati. NULL se non ci sono elementi da restituire.
 */
Tournament* get_tournaments(const AppContext* app_context, FilterTournamentState state_filter, size_t* tournaments_count);

/**
 * Funzione di Servizio per ottenere una lista di copie di partecipazioni.
 * @warning L'array di tornei restituito è allocato dinamicamente a causa della quantità variabile di risultati -
 * è OBBLIGATORIO rilasciare la memoria usando free() sul valore restituito da questo metodo per prevenire un memory leak!
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tournament_id L'ID del torneo per il quale ottenere le partecipazioni
 * @param participations_count Il puntatore alla variabile che conterrà il numero di partecipazioni.
 * Sarà impostata a SIZE_MAX se il sistema non ha memoria sufficiente per eseguire l'operazione (OOM).
 * @return Un array delle copie delle partecipazioni al torneo specificato. NULL se non ci sono elementi da restituire.
 */
TournamentParticipation* get_tournament_participations(const AppContext* app_context, uint32_t tournament_id, size_t* participations_count);

/**
 * Funzione di Servizio per ottenere una lista di copie dei tennisti che partecipano al torneo specificato.
 * @warning L'array di tornei restituito è allocato dinamicamente a causa della quantità variabile di risultati -
 * è OBBLIGATORIO rilasciare la memoria usando free() sul valore restituito da questo metodo per prevenire un memory leak!
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tournament_id L'ID del torneo richiesto
 * @param players_count Il puntatore alla variabile che conterrà il numero di tennisti.
 * Sarà impostata a SIZE_MAX se non c'è abbastanza memoria nel sistema per completare l'operazione
 * @return Un array delle copie dei giocatori che partecipano al torneo specificato. NULL se non ci sono elementi da restituire.
 */
TennisPlayer* get_tournament_players(const AppContext* app_context, uint32_t tournament_id, size_t* players_count);

/**
 * Funzione di Servizio per ottenere una lista di copie dei tennisti che NON partecipano al torneo specificato.
 * @warning L'array di tornei restituito è allocato dinamicamente a causa della quantità variabile di risultati -
 * è OBBLIGATORIO rilasciare la memoria usando free() sul valore restituito da questo metodo per prevenire un memory leak!
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tournament_id L'ID del torneo richiesto
 * @param players_count Il puntatore alla variabile che conterrà il numero di tennisti.
 * Sarà impostata a SIZE_MAX se non c'è abbastanza memoria nel sistema per completare l'operazione
 * @return Un array delle copie dei giocatori che NON partecipano al torneo specificato. NULL se non ci sono elementi da restituire.
 */
TennisPlayer* get_players_not_in_tournament(const AppContext* app_context, uint32_t tournament_id, size_t* players_count);

/**
 * Funzione di Servizio che restituisce il numero di tennisti partecipanti ad un torneo specificato.
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tournament_id L'ID del torneo interessato
 * @return Il numero di tennisti partecipanti al torneo specificato in tournament_id.
 */
size_t get_tournament_players_quantity(const AppContext* app_context, uint32_t tournament_id);

/**
 * Funzione di Servizio che restituisce una copia del Torneo il cui ID corrisponde a quello specificato
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param id L'ID del torneo da cercare
 * @param tournament Il puntatore alla variabile nella quale verranno copiati i dettagli del torneo
 * @return true se la ricerca è avvenuta con successo, false se non è stato trovato alcun torneo con l'ID richiesto
 */
bool get_tournament_by_id(const AppContext* app_context, uint32_t id, Tournament* tournament);

/**
 * Funzione di Servizio per aggiungere un Torneo al sistema.
 * @param app_context Il puntatore al contesto dell'applicazione.
 * @param new_tournament Il puntatore allo struct del nuovo Torneo da aggiungere. I dati lì presenti saranno clonati ed inseriti nella tabella
 * @param out_id Il puntatore ad una variabile size_t che conterrà l'ID del Torneo appena inserito
 * @return L'esito dell'operazione (ServiceStatus).
 * - STATUS_ERR_UNAUTHORIZED se l'utente non ha i privilegi necessari per questa operazione
 * - STATUS_ERR_TITLE_EMPTY se il Torneo non ha un titolo (non permesso)
 * - STATUS_ERR_OOM se il sistema ha esaurito la memoria e non può aggiungere ulteriori dati
 * - STATUS_OK se l'operazione è avvenuta con successo
 */
ServiceStatus add_tournament(AppContext* app_context, Tournament* new_tournament, uint32_t* out_id);

/**
 * Funzione di Servizio per modificare un Torneo nel sistema.
 * @param app_context Il puntatore al contesto dell'applicazione.
 * @param edited_tournament Il puntatore allo struct del Torneo con i dati modificati. I dati lì presenti saranno clonati ed inseriti nella tabella
 * @return L'esito dell'operazione (ServiceStatus)
 * - STATUS_ERR_UNAUTHORIZED se l'utente non ha i privilegi necessari per questa operazione
 * - STATUS_ERR_TITLE_EMPTY se il Torneo non ha un titolo (non permesso)
 * - STATUS_ERR_NOT_FOUND se l'ID nella struttura non corrisponde ad alcun torneo attualmente nel sistema
 * - STATUS_ERR_NOT_ENOUGH_PLAYERS se lo stato è stato cambiato da Aperto a In corso, e non partecipano almeno due tennisti
 * - STATUS_ERR_TOURNAMENT_ALREADY_STARTED se lo stato è stato cambiato da In corso ad Aperto ed è stata registrata almeno una partita
 * - STATUS_ERR_TOURNAMENT_NOT_STARTED se lo stato è stato cambiato da In corso a Completo e non è stata registrata neanche una partita
 * - STATUS_ERR_WINNER_NOT_SET se lo stato è stato cambiato da In corso a Completo e non è stato segnato un vincitore
 * - STATUS_ERR_ILLEGAL_TRANSITION se è stato effettuato un cambio di stato non permesso
 * - STATUS_ERR_OOM se il sistema ha esaurito la memoria (ne è richiesta un pò caricare dati riguardanti il vincitore, se impostato)
 * - STATUS_ERR_INVALID_WINNER se è stato specificato un vincitore che non partecipa al torneo
 * - STATUS_OK se l'operazione è avvenuta con successo
 */
ServiceStatus edit_tournament(AppContext* app_context, Tournament* edited_tournament);

/**
 * Funzione di Servizio per eliminare un Torneo nel sistema.
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param id L'ID del Torneo da eliminare
 * @note Questa funzione eliminerà a cascata anche tutte le partecipazioni, le squadre e le partite legate al torneo che sta venendo eliminato!
 * @return L'esito dell'operazione (ServiceStatus)
 * - STATUS_ERR_UNAUTHORIZED se l'utente non ha i privilegi necessari per questa operazione
 * - STATUS_ERR_OOM se il sistema ha esaurito la memoria (ne è richiesta un pò per caricare tutti gli altri dati legati al torneo da eliminare!!)
 * - STATUS_OK se l'operazione è avvenuta con successo
 */
ServiceStatus delete_tournament(AppContext* app_context, uint32_t id);

/**
 * Funzione di Servizio per aggiungere una partecipazione ad un Torneo nel sistema
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param player_id L'ID del Tennista partecipante
 * @param tournament_id L'ID del Torneo al quale partecipa il Tennista
 * @return L'esito dell'operazione (ServiceStatus)
 * - STATUS_ERR_UNAUTHORIZED se l'utente non ha i privilegi necessari per questa operazione
 * - STATUS_ERR_PLAYER_NOT_FOUND se l'ID del Tennista specificato non corrisponde ad alcun Tennista nel sistema
 * - STATUS_ERR_TOURNAMENT_NOT_FOUND se l'ID del Torneo specificato non corrisponde ad alcun Torneo nel sistema
 * - STATUS_ERR_PLAYER_UNAVAILABLE se il Tennista è impostato come non disponibile; di conseguenza non può partecipare ad un torneo
 * - STATUS_ERR_TOURNAMENT_ALREADY_STARTED se il Torneo non è aperto; non si possono aggiungere Tennisti ad un Torneo già iniziato!
 * - STATUS_ERR_PLAYER_ALREADY_PARTICIPATING se il Tennista partecipa già al torneo
 * - STATUS_ERR_OOM se il sistema ha esaurito la memoria
 * - STATUS_OK se l'operazione è avvenuta con successo
 */
ServiceStatus add_tournament_participation(AppContext* app_context, uint32_t player_id, uint32_t tournament_id);

/**
 * Funzione di Servizio per rimuovere una partecipazione ad un Torneo nel sistema
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param player_id L'ID del Tennista partecipante
 * @param tournament_id L'ID del Torneo al quale partecipa il Tennista
 * @return L'esito dell'operazione (ServiceStatus)
 * - STATUS_ERR_UNAUTHORIZED se l'utente non ha i privilegi necessari per questa operazione
 * - STATUS_ERR_PLAYER_NOT_FOUND se l'ID del Tennista specificato non corrisponde ad alcun Tennista nel sistema
 * - STATUS_ERR_TOURNAMENT_NOT_FOUND se l'ID del Torneo specificato non corrisponde ad alcun Torneo nel sistema
 * - STATUS_ERR_TOURNAMENT_ALREADY_STARTED se il Torneo non è aperto; non si possono rimuovere Tennisti da un Torneo già iniziato!
 * - STATUS_ERR_NOT_FOUND se non è stata trovata la partecipazione che si desidera eliminare
 * - STATUS_OK se l'operazione è avvenuta con successo
 */
ServiceStatus delete_tournament_participation(AppContext* app_context, uint32_t player_id, uint32_t tournament_id);

/**
 * Funzione di Servizio per iscrivere un Utente ad un Torneo nel sistema
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tournament_id L'ID del Torneo al quale si sta iscrivendo l'utente
 * @return L'esito dell'operazione (ServiceStatus)
 * - STATUS_ERR_UNAUTHORIZED se l'utente non ha i privilegi necessari per questa operazione
 * - STATUS_ERR_TOURNAMENT_NOT_FOUND se l'ID del Torneo specificato non corrisponde ad alcun Torneo nel sistema
 * - STATUS_ERR_TOURNAMENT_NOT_OPEN se il Torneo non è aperto; non ci si può più iscrivere a Tornei che sono già iniziati
 * - STATUS_ERR_USER_ALREADY_SIGNED_UP se l'Utente è gia iscritto al torneo
 * - STATUS_ERR_OOM se il sistema ha esaurito la memoria
 * - STATUS_OK se l'operazione è avvenuta con successo
 */
ServiceStatus sign_up_tournament(AppContext* app_context, uint32_t tournament_id);

#endif
