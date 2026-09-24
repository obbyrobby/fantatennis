/**
 * @file cli_startuperr.h
 * @brief Modulo di CLI per mostrare errori avvenuti all'avvio dell'applicazione all'utente
 * @author Roberto Massante e Martina Gisonda
*/

#ifndef FANTATENNIS_CLI_STARTUPERR_H
#define FANTATENNIS_CLI_STARTUPERR_H

#include "../entity_table.h"

/**
 * Funzione di CLI che mostra il messaggio di errore appropriato all'utente in base all'esatto tipo di errore avvenuto.
 * @param result L'enum ReadFileResult contenente l'errore
 * @param bad_file La stringa contenente il nome del file che ha causato errori
 */
void handle_startuperr(ReadFileResult result, char* bad_file);

#endif
