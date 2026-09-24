/**
 * @file app_context.h
 * @brief Modulo della definizione dell'entità che raccoglie l'intero contesto dell'applicazione
 * @author Roberto Massante e Martina Gisonda
 */

#ifndef FANTATENNIS_APP_CONTEXT_H
#define FANTATENNIS_APP_CONTEXT_H

#include "entity_table.h"

/**
 * @brief Struttura che raccoglie tutte le entità del sistema, più dati sulla sessione attuale.
 * @note Inizializzare con init_app_context, e distruggere con destroy_app_context per rilasciare tutta la memoria occupata dalle tabelle.
 * @details Funge da punto di accesso centrale per il layer dei servizi e la CLI, eliminando le variabili
 * globali e garantendo un ciclo di vita esplicito per le risorse dinamiche.
 */
typedef struct {
    EntityTable users_table;                /**< La tabella degli Utenti */
    EntityTable tournaments_table;          /**< La tabella dei Tornei   */
    EntityTable tennisplayers_table;        /**< La tabella dei Tennisti */
    EntityTable teams_table;                /**< La tabella delle Squadre */
    EntityTable matches_table;              /**< La tabella delle Partite */
    EntityTable participations_table;       /**< La tabella delle Partecipazioni dei Tennisti ai Tornei */

    uint32_t current_user_id;               /**< L'ID dell'Utente attualmente loggato nel sistema */
    bool is_admin;                          /**< Flag che indica se l'utente attualmente loggato è un amministratore */
} AppContext;

/**
 * Inizializza il contesto dell'applicazione.
 * @details Carica tutte le tabelle di tutte le entità da disco in memoria, e crea l'utente admin di default se non esiste.
 * Se il caricamento non va a buon fine, automaticamente rilascia la memoria usata dalle tabelle.
 * @param app_context Il puntatore al contesto vuoto dell'applicazione
 * @param bad_file Puntatore alla stringa nella quale, nel caso di un caricamento andato storto, verrà inserito il nome del file problematico.
 * @return L'esito della lettura dei file (ReadFileResult).
 */
ReadFileResult init_app_context(AppContext* app_context, char* bad_file);

/**
 * Distrugge il contesto dell'applicazione.
 * @details Rilascia tutta la memoria usata dalle tabelle delle entità.
 * @param app_context Il puntatore al contesto dell'applicazione
 */
void destroy_app_context(AppContext* app_context);

#endif
