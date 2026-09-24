/**
 * @file tennis_player.h
 * @brief Modulo della definizione del modello TennisPlayer
 * @author Roberto Massante e Martina Gisonda
 */

#ifndef FANTATENNIS_TENNISTA_H
#define FANTATENNIS_TENNISTA_H
#include <stdbool.h>
#include <stdint.h>

#include "../config.h"

/**
 * @brief Rappresenta un Tennista.
 */
typedef struct {
    uint32_t id;                                    /**< L'ID unico del Tennista */
    char name[TENNISPLAYER_NAME_SIZE];              /**< Il nome del Tennista */
    char surname[TENNISPLAYER_SURNAME_SIZE];        /**< Il cognome del Tennista */
    bool is_available;                              /**< La disponibilità del Tennista */
    int ranking;                                    /**< Il ranking del Tennista (impostato dagli amministratori, non calcolato dinamicamente) */
} TennisPlayer;

#endif
