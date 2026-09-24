/**
 * @file cli_leaderboards.h
 * @brief Modulo di CLI per le interazioni riguardanti le classifiche
 * @author Roberto Massante e Martina Gisonda
*/

#ifndef FANTATENNIS_CLI_LEADERBOARDS_H
#define FANTATENNIS_CLI_LEADERBOARDS_H

#include "../../app_context.h"
#include "../../../include/common_types.h"

/**
 * Funzione di CLI riutilizzabile per stampare su terminale una classifica di Tennisti
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param scores L'array dei punteggi PlayerScore
 * @param scores_count La quantità dei punteggi nell'array
 */
void cli_print_tennisplayers_leaderboard(const AppContext* app_context, const PlayerScore* scores, size_t scores_count);

/**
 * Funzione di CLI riutilizzabile per stampare su terminale una classifica di Fanta-Coach
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param scores L'array dei punteggi UserScore
 * @param scores_count La quantità dei punteggi nell'array
 */
void cli_print_users_leaderboard(const AppContext* app_context, const UserScore* scores, size_t scores_count);

/**
 * Funzione di CLI che mostra all'utente la classifica globale dei Tennisti
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param data Inutilizzato
 */
void cli_global_tennisplayers_leaderboard(AppContext* app_context, void* data);

/**
 * Funzione di CLI che mostra all'utente la classifica globale dei Fanta-Coach
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param data Inutilizzato
 */
void cli_global_users_leaderboard(AppContext* app_context, void* data);

#endif
