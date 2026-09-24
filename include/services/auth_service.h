/**
 * @file auth_service.h
 * @brief Modulo dei servizi posti a disposizione per l'autenticazione e l'interazione con i dati sugli Utenti
 * @author Roberto Massante e Martina Gisonda
*/

#ifndef FANTATENNIS_AUTH_SERVICE_H
#define FANTATENNIS_AUTH_SERVICE_H

#include "../app_context.h"
#include "../models/user.h"
#include "../common_types.h"

/**
 * @brief Possibili esiti di Login
 */
typedef enum {
    LOGIN_SUCCESS,
    LOGIN_USERNAME_NOT_FOUND,
    LOGIN_WRONG_PASSWORD
} LoginResult;

/**
 * @brief Possibili esiti di Registrazione
 */
typedef enum {
    REGISTER_SUCCESS,
    REGISTER_USERNAME_ALREADY_TAKEN,
    REGISTER_USERNAME_TOO_SHORT,
    REGISTER_PASSWORD_TOO_SHORT,
    REGISTER_FAILURE
} RegisterResult;

/**
 * Funzione di Servizio per eseguire il Login nel sistema date le credenziali.
 * @details Se il Login avviene con successo, l'ID dell'Utente verrà inserito nel contesto corrente dell'applicazione (AppContext).
 * Altrimenti, la funzione restituirà un enum LoginResult con il valore che identifica l'errore.
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param username L'username dell'utente
 * @param password La password dell'utente
 * @return L'esito dell'operazione (LoginResult).
 */
LoginResult try_login(AppContext* app_context, const char* username, const char* password);

/**
 * Funzione di Servizio per eseguire la registrazione di un utente nel sistema date le sue nuove credenziali.
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param username L'username del nuovo utente
 * @param password La password del nuovo utente
 * @return L'esito dell'operazione (RegisterResult).
 */
RegisterResult try_register(AppContext* app_context, const char* username, const char* password);

/**
 * Funzione di Servizio per eseguire il Logout dal sistema.
 * @details Resetta l'ID dell'utente e la flag di amministratore nel contesto dell'applicazione (AppContext).
 * @param app_context Il puntatore al contesto dell'applicazione
 */
void logout(AppContext* app_context);

/**
 * Funzione di Servizio che restituisce la copia dell'Utente il cui ID corrisponde a quello specificato
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param id L'ID dell'utente da cercare
 * @param user Il puntatore alla variabile nella quale verranno copiati i dettagli dell'utente
 * @return true se la ricerca è avvenuta con successo, false se non è stato trovato alcun utente con l'ID specificato
 */
bool get_user_by_id(const AppContext* app_context, uint32_t id, User* user);

/**
 * Funzione di Servizio per ottenere una lista di copie di utenti.
 * @warning L'array restituito è allocato dinamicamente a causa della quantità variabile di risultati -
 * è OBBLIGATORIO rilasciare la memoria usando free() sul valore restituito da questo metodo per prevenire un memory leak!
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param include_admins Specifica se includere gli amministratori nella ricerca o meno
 * @param users_count Il puntatore alla variabile che conterrà il numero di utenti restituiti dalla funzione.
 * Sarà impostata a SIZE_MAX se il sistema non ha memoria sufficiente per eseguire l'operazione (OOM).
 * @return Un array delle copie degli utenti che rientrano nei criteri specificati. NULL se non ci sono elementi da restituire.
 */
User* get_users(const AppContext* app_context, bool include_admins, size_t* users_count);

/**
 * Funzione di Servizio che imposta un utente come amministratore.
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param user_id L'ID dell'Utente da rendere amministratore
 * @return L'esito dell'operazione (ServiceStatus).
 * - STATUS_ERR_UNAUTHORIZED se l'utente non ha i privilegi necessari per questa operazione
 * - STATUS_ERR_USER_NOT_FOUND se l'ID specificato non appartiene ad alcun utente
 * - STATUS_OK se l'operazione è avvenuta con successo
 */
ServiceStatus set_admin(AppContext* app_context, uint32_t user_id);

#endif
