/**
 * @file string_utils.h
 * @brief Modulo di funzioni di utilità per operazioni su stringhe
 * @author Roberto Massante e Martina Gisonda
*/

#ifndef FANTATENNIS_STRING_UTILS_H
#define FANTATENNIS_STRING_UTILS_H

#include "stdbool.h"

/**
 * Controlla se una stringa è sottostringa di un'altra.
 * @note Il controllo è case-insensitive! Differenze in maiuscolo/minuscolo sono ignorate durante la ricerca
 * @param str La stringa dove cercare
 * @param substr La sottostringa da ricercare all'interno di str
 * @return true se substr è contenuta in str, altrimenti false
 */
bool str_contains(const char* str, const char* substr);

#endif

