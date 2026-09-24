/**
 * @file user.h
 * @brief Modulo della definizione del modello User
 * @author Roberto Massante e Martina Gisonda
 */

#ifndef FANTATENNIS_UTENTE_H
#define FANTATENNIS_UTENTE_H
#include <stdbool.h>
#include <stdint.h>
#include "../config.h"

/**
 * @brief Rappresenta un Utente.
 */
typedef struct {
    uint32_t id;                        /**< L'ID unico dell'Utente */
    char username[USERNAME_SIZE];       /**< L'username dell'Utente */
    char password[PASSWORD_SIZE];       /**< La password dell'Utente */
    bool is_admin;                      /**< Flag che indica se l'utente è un amministratore o meno.*/
} User;

/**
 * Costruttore per un oggetto di tipo User.
 * @param username L'username del nuovo utente
 * @param password La password del nuovo utente
 * @param is_admin Specifica se il nuovo utente sarà un amministratore o meno
 * @return L'oggetto User assemblato con tutti i parametri assegnati
 */
User user_build(const char* username, const char* password, bool is_admin);

#endif
