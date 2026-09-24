/**
 * @file system_service.h
 * @brief Modulo per Servizi di sistema che operano su molteplici entità in maniera generica
 * @author Roberto Massante e Martina Gisonda
 */

#ifndef FANTATENNIS_SYSTEM_SERVICE_H
#define FANTATENNIS_SYSTEM_SERVICE_H

#include <stdbool.h>
#include "../app_context.h"

/**
 * Funzione di Servizio che verifica se il sistema è completamente privo di dati, a parte l'utente amministratore di default.
 * @param app_context Il puntatore al contesto dell'applicazione
 * @return true se il sistema è vuoto, altrimenti false
 */
bool is_system_empty(const AppContext* app_context);

/**
 * Funzione di Servizio che azzera l'intero sistema, cancellando tutti i dati in ogni tabella
 * @warning Funzione pericolosa!! Usare con estrema cautela. L'operazione è irreversibile
 * @param app_context Il puntatore al contesto dell'applicazione
 */
void clear_system(AppContext* app_context);

#endif
