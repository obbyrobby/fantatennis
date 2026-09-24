/**
 * @file common_types.h
 * @brief Modulo contenente vari tipi di dato condivisi tra vari layer dell'applicazione
 * @author Roberto Massante e Martina Gisonda
*/

#ifndef FANTATENNIS_COMMON_TYPES_H
#define FANTATENNIS_COMMON_TYPES_H

#include "models/tournament.h"

/**
 * @brief Enum per i filtri booleani
 */
typedef enum {
    FILTER_TRUE = 1,
    FILTER_FALSE = 2,
    FILTER_NOT_SET = 3
} FilterBool;

/**
 * @brief Enum per il filtro dello stato di un torneo
 */
typedef enum {
    FILTER_TOURNAMENT_STATE_ALL = 0,
    FILTER_TOURNAMENT_STATE_OPEN = TOURNAMENT_STATE_OPEN,
    FILTER_TOURNAMENT_STATE_ONGOING = TOURNAMENT_STATE_ONGOING,
    FILTER_TOURNAMENT_STATE_COMPLETED = TOURNAMENT_STATE_COMPLETED
} FilterTournamentState;

/**
 * @brief Enum per ogni possibile esito delle funzioni di Servizio
 */
typedef enum {
    STATUS_OK,
    STATUS_ERR_UNAUTHORIZED,
    STATUS_ERR_NOT_FOUND,
    STATUS_ERR_OOM,
    STATUS_ERR_PLAYER_BUSY,
    STATUS_ERR_PLAYER_HAS_HISTORY,
    STATUS_ERR_NAME_EMPTY,
    STATUS_ERR_SURNAME_EMPTY,
    STATUS_ERR_TITLE_EMPTY,
    STATUS_ERR_NOT_ENOUGH_PLAYERS,
    STATUS_ERR_TOURNAMENT_ALREADY_STARTED,
    STATUS_ERR_TOURNAMENT_NOT_STARTED,
    STATUS_ERR_ILLEGAL_TRANSITION,
    STATUS_ERR_PLAYER_NOT_FOUND,
    STATUS_ERR_TOURNAMENT_NOT_FOUND,
    STATUS_ERR_PLAYER_UNAVAILABLE,
    STATUS_ERR_PLAYER_ALREADY_PARTICIPATING,
    STATUS_ERR_WINNER_NOT_SET,
    STATUS_ERR_INVALID_WINNER,
    STATUS_ERR_RANKING_NOT_SET,
    STATUS_ERR_TIE_NOT_ALLOWED,
    STATUS_ERR_INVALID_PLAYERS,
    STATUS_ERR_TOURNAMENT_NOT_ONGOING,
    STATUS_ERR_SAME_PLAYERS,
    STATUS_ERR_USER_ALREADY_SIGNED_UP,
    STATUS_ERR_TOURNAMENT_NOT_OPEN,
    STATUS_ERR_USER_NOT_SIGNED_UP,
    STATUS_ERR_PLAYER_NOT_IN_TOURNAMENT,
    STATUS_ERR_TEAM_NOT_FILLED,
    STATUS_ERR_BUDGET_EXCEEDED,
    STATUS_ERR_PLAYER_DUPED,
    STATUS_ERR_USER_NOT_FOUND
} ServiceStatus;

/**
 * @brief Struttura per contenere l'ID di un Tennista e il suo punteggio
 */
typedef struct {
    uint32_t tennisplayer_id;
    double score;
} PlayerScore;

/**
 * @brief Struttura per contenere l'ID di un Utente/Fanta-Coach e il suo punteggio
 */
typedef struct {
    uint32_t user_id;
    double score;
} UserScore;

#endif
