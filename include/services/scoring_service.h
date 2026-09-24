/**
 * @file scoring_service.h
 * @brief Modulo dei servizi posti a disposizione per il calcolo dei punteggi di Tennisti e Fanta-Coach
 * @author Roberto Massante e Martina Gisonda
*/

#ifndef FANTATENNIS_SCORING_SERVICE_H
#define FANTATENNIS_SCORING_SERVICE_H

#include "../models/match.h"
#include "../app_context.h"
#include "../common_types.h"

/**
 * Funzione di Servizio per calcolare i punti ottenuti da un Tennista in una singola Partita
 * @param match Il puntatore alla Partita in questione
 * @param player_index 0 per calcolare i punti del primo Tennista, 1 per il secondo. Ogni altro valore sarà invalido e farà restituire 0 alla funzione
 * @return Il punteggio ottenuto.
 */
double get_match_score(const Match* match, unsigned int player_index);

/**
 * Funzione di Servizio per calcolare i punti ottenuti da un Tennista in un Torneo
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tournament_id L'ID del Torneo in questione
 * @param tennisplayer_id L'ID del Tennista per il quale calcolare il punteggio
 * @return Il punteggio ottenuto
 */
double get_player_tournament_score(const AppContext* app_context, uint32_t tournament_id, uint32_t tennisplayer_id);

/**
 * Funzione di Servizio per calcolare i punti ottenuti da un Fanta-Coach in un Torneo
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tournament_id L'ID del Torneo in questione
 * @param user_id L'ID del Fanta-Coach per il quale calcolare il punteggio
 * @return Il punteggio ottenuto
 */
double get_user_tournament_score(const AppContext* app_context, uint32_t tournament_id, uint32_t user_id);

/**
 * Funzione di Servizio per calcolare la somma dei punti ottenuti da un Tennista in ogni torneo nel sistema
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tennisplayer_id L'ID del Tennista per il quale calcolare il punteggio
 * @return Il punteggio ottenuto
 */
double get_player_global_score(const AppContext* app_context, uint32_t tennisplayer_id);

/**
 * Funzione di Servizio per calcolare la somma dei punti ottenuti da un FantaCoach in ogni torneo nel sistema
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param user_id L'ID del FantaCoach per il quale calcolare il punteggio
 * @return Il punteggio ottenuto
 */
double get_user_global_score(const AppContext* app_context, uint32_t user_id);

/**
 * Funzione di Servizio per ottenere la classifica dei punteggi dei Tennisti di un Torneo.
 * @warning L'array dei punteggi restituito è allocato dinamicamente a causa della quantità variabile di risultati -
 * è OBBLIGATORIO rilasciare la memoria usando free() sul valore restituito da questo metodo per prevenire un memory leak!
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tournament_id L'ID del Torneo in questione
 * @param out_count Il puntatore alla variabile che conterrà il numero di punteggi restituiti dalla funzione.
 * Sarà impostata a SIZE_MAX se non c'è abbastanza memoria nel sistema per completare l'operazione
 * @return Un array di PlayerScore, contenenti ciascuno l'ID del Tennista e il rispettivo Punteggio numerico.
 */
PlayerScore* get_tournament_players_leaderboard(const AppContext* app_context, uint32_t tournament_id, size_t* out_count);

/**
 * Funzione di Servizio per ottenere la classifica dei punteggi dei Fanta-Coach di un Torneo.
 * @warning L'array dei punteggi restituito è allocato dinamicamente a causa della quantità variabile di risultati -
 * è OBBLIGATORIO rilasciare la memoria usando free() sul valore restituito da questo metodo per prevenire un memory leak!
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param tournament_id L'ID del Torneo in questione
 * @param out_count Il puntatore alla variabile che conterrà il numero di punteggi restituiti dalla funzione.
 * Sarà impostata a SIZE_MAX se non c'è abbastanza memoria nel sistema per completare l'operazione
 * @return Un array di UserScore, contenenti ciascuno l'ID del FantaCoach e il rispettivo Punteggio numerico.
 */
UserScore* get_tournament_users_leaderboard(const AppContext* app_context, uint32_t tournament_id, size_t* out_count);

/**
 * Funzione di Servizio per ottenere la classifica globale dei punteggi dei Tennisti, sommandoli da ogni torneo nel sistema
 * @warning L'array dei punteggi restituito è allocato dinamicamente a causa della quantità variabile di risultati -
 * è OBBLIGATORIO rilasciare la memoria usando free() sul valore restituito da questo metodo per prevenire un memory leak!
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param out_count Il puntatore alla variabile che conterrà il numero di punteggi restituiti dalla funzione.
 * Sarà impostata a SIZE_MAX se non c'è abbastanza memoria nel sistema per completare l'operazione
 * @return Un array di PlayerScore, contenenti ciascuno l'ID del Tennista e il rispettivo Punteggio numerico.
 */
PlayerScore* get_global_players_leaderboard(const AppContext* app_context, size_t* out_count);

/**
 * Funzione di Servizio per ottenere la classifica globale dei punteggi dei Fanta-Coach, sommandoli da ogni torneo nel sistema
 * @warning L'array dei punteggi restituito è allocato dinamicamente a causa della quantità variabile di risultati -
 * è OBBLIGATORIO rilasciare la memoria usando free() sul valore restituito da questo metodo per prevenire un memory leak!
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param out_count Il puntatore alla variabile che conterrà il numero di punteggi restituiti dalla funzione.
 * Sarà impostata a SIZE_MAX se non c'è abbastanza memoria nel sistema per completare l'operazione
 * @return Un array di UserScore, contenenti ciascuno l'ID del FantaCoach e il rispettivo Punteggio numerico.
 */
UserScore* get_global_users_leaderboard(const AppContext* app_context, size_t* out_count);

#endif
