/**
 * @file tournament.h
 * @brief Modulo della definizione del modello Tournament
 * @author Roberto Massante e Martina Gisonda
 */

#ifndef FANTATENNIS_TOURNAMENT_H
#define FANTATENNIS_TOURNAMENT_H

// C non ha namespaces che separano i valori degli enum, quindi è bene sempre aggiungere prefissi

/**
 * @brief Rappresenta lo stato di un Torneo.
 */
typedef enum {
    TOURNAMENT_STATE_OPEN = 1,          /**< Aperto: iscrizioni sono aperte, è ancora possibile modificare i partecipanti, impostare le squadre. Non ci sono ancora partite giocate. */
    TOURNAMENT_STATE_ONGOING = 2,       /**< In Corso: iscrizioni sono chiuse, non si possono più modificare partecipanti. Si possono aggiungere partite ed esiti. */
    TOURNAMENT_STATE_COMPLETED = 3      /**< Completato: il torneo è finito, non si può aggiungere o modificare più nulla, ed è stato proclamato un vincitore. */
} TournamentState;

/**
 * @brief Rappresenta un Torneo.
 */
typedef struct {
    uint32_t id;                        /**< L'ID unico del Torneo */
    char title[50];                     /**< Il titolo del Torneo */
    TournamentState state;              /**< Lo stato del Torneo */
    uint32_t winner_id;                 /**< Se il torneo è Completato, l'ID del Tennista che ha vinto */
} Tournament;


/**
 * @brief Rappresenta la partecipazione di un Tennista ad un Torneo.
 * @details Funge da entità associativa tra Tennisti e Tornei. E' una relazione del tipo Many-to-Many, tanti tennisti possono
 * iscriversi a tanti tornei diversi, ragion per cui è richiesta questa tabella di giunzione.
 * @see TennisPlayer
 * @see Tournament
 */
typedef struct {
    uint32_t id;                    /**< L'ID unico della Partecipazione */
    uint32_t tennisplayer_id;       /**< L'ID del Tennista partecipante */
    uint32_t tournament_id;         /**< L'ID del Torneo al quale partecipa il Tennista */
} TournamentParticipation;

#endif
