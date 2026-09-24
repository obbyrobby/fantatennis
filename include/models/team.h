/**
 * @file team.h
 * @brief Modulo della definizione del modello Team
 * @author Roberto Massante e Martina Gisonda
 */

#ifndef FANTATENNIS_TEAM_H
#define FANTATENNIS_TEAM_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../config.h"

/**
 * @brief Rappresenta una Squadra. Fa anche da entità associativa/tabella ponte tra le entità User e Tournament,
 * fungendo da iscrizione di un FantaCoach ad un Torneo.
 */
typedef struct {
    uint32_t id;                            /**< L'ID unico della Squadra */
    uint32_t players_ids[TEAM_SIZE];        /**< Gli ID dei Tennisti appartenenti a questa Squadra */
    uint32_t user_id;                       /**< L'ID dell'Utente/Fanta-Coach a cui appartiene questa Squadra */
    uint32_t tournament_id;                 /**< L'ID del Torneo per il quale è stata creata questa Squadra */
} Team;

/**
 * Funzione helper che controlla se la squadra è stata configurata, ovvero che player_ids sia popolata fino all'ultimo indice.
 * @param team Il puntatore alla Squadra da controllare
 * @return true se è configurata, false se non lo è
 */
bool is_team_set(const Team* team);

#endif
