/**
 * @file match.h
 * @brief Modulo della definizione dei modelli Match e PlayerResult
 * @author Roberto Massante e Martina Gisonda
 */

#ifndef FANTATENNIS_MATCH_H
#define FANTATENNIS_MATCH_H

#include <stdlib.h>
#include <stdint.h>

/**
 * @brief Rappresenta esito e dati di un Tennista ad una Partita.
 * @details Questa struttura vive all'interno di Match in una quantità fissa; ce ne sono sempre due per Match, una per Tennista.
 * Per questa ragione, non ha bisogno della sua EntityTable.
 */
typedef struct {
    uint32_t player_id; /**< L'ID del Tennista a cui appartengono questi risultati */
    int won_sets;           /**< Il numero di set vinti */
    int aces;               /**< Numero di Ace */
    int double_faults;      /**< Numero di Doppi Falli */
    int breaks;             /**< Numero di Break */
} PlayerResult;

/**
 * @brief Rappresenta una Partita.
 */
typedef struct {
    uint32_t id;                        /**< L'ID unico della partita */
    uint32_t tournament_id;             /**< L'ID del Torneo nel quale è avvenuta questa partita */
    PlayerResult player_results[2];     /**< Esiti e risultati dei due Tennisti che si sono sfidati in questa partita */
} Match;

/**
 * Restituisce l'indice in player_results del vincitore della partita, in base al numero di Set vinti.
 * @param match Il puntatore alla Partita da controllare
 * @return -1 se non viene passato il puntatore ad una Partita; 0 se ha vinto il primo Tennista, 1 se ha vinto il secondo.
 */
int match_get_winner_index(const Match* match);

#endif
