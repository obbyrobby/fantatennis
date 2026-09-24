/**
 * @file cli_utils.h
 * @brief Modulo di CLI contenente funzioni di utilità per interazioni con l'utente
 * @author Roberto Massante e Martina Gisonda
 */

#ifndef FANTATENNIS_CLI_UTILS_H
#define FANTATENNIS_CLI_UTILS_H

#ifdef _WIN32
#include <windows.h>
#endif
#include <stdbool.h>
#include "../../include/app_context.h"
#include "../../include/models/tournament.h"
#include "../common_types.h"


/**
 * @brief Rappresenta una funzione da eseguire dal menu (in cli_run_menu()). Ogni funzione lì presente dovrà avere gli stessi parametri.
 * @param app_context Puntatore al contesto dell'applicazione.
 * @param data Puntatore a dati extra se necessari.
 */
typedef void (*MenuAction)(AppContext *app_context, void* data);

/**
 * @brief Rappresenta una voce all'interno del menu (in cli_run_menu()).
 */
typedef struct {
    const char* description;    /**< La didascalia dell'opzione */
    MenuAction action;          /**< Il puntatore alla funzione da eseguire quando l'opzione viene scelta */
} MenuOption;

/**
 * Legge numeri interi da tastiera in maniera sicura
 * @param min Il limite inferiore del range dei valori da accettare
 * @param max Il limite superiore del range dei valori da accettare
 * @param value Il puntatore alla variabile nella quale inserire il numero letto da tastiera
 * @param allow_cancel Booleano che permette o nega l'annullamento della lettura se l'utente inserisce un input vuoto
 * @param show_message Booleano che imposta se la funzione scrive su console una descrizione della richiesta all'utente o meno
 * @return Vero se un numero è stato letto con successo, falso se l'operazione non è stata completata
 */
bool cli_read_int(int min, int max, int* value, bool allow_cancel, bool show_message);

/**
 * Legge stringhe da tastiera in maniera sicura
 * @param str Il puntatore alla variabile nella quale inserire la stringa letta da tastiera
 * @param len La lunghezza massima della stringa
 * @return Vero se una stringa è stata letta con successo, falso se l'operazione non è stata completata
 */
bool cli_read_string(char* str, int len);


/**
 * Fa selezionare all'utente un'opzione da una lista di possibili opzioni
 * @param options L'array di stringhe che rappresentano le possibili scelte
 * @param count Il numero di possibili scelte (deve corrispondere alla dimensione di options!)
 * @param choice Il puntatore alla variabile nella quale verrà inserito l'indice dell'opzione scelta
 * @param allow_cancel Booleano che permette o nega l'annullamento della scelta se l'utente inserisce un input vuoto
 * @return Vero se un'opzione è stata selezionata con successo, falso se l'operazione è stata annullata
 */
bool cli_pick_option(const char** options, int count, int* choice, bool allow_cancel);


/**
 * Mostra un menu all'utente, permette la selezione, ed esegue la funzione scelta. Include anche un opzione per uscire dal menù, e da lì la funzione ritorna.
 * @param app_context Il puntatore al contesto dell'applicazione
 * @param options L'array di possibili opzioni (ognuna della quale contenente una descrizione e un puntatore alla funzione da eseguire)
 * @param count Il numero di possibili opzioni (deve corrispondere alla dimensione di options!)
 * @param quit_caption La didascalia per la voce di uscita dal menu
 * @param data Un puntatore a dati extra da passare se necessari
 */
void cli_run_menu(AppContext* app_context, const MenuOption* options, int count, const char* quit_caption, void* data);

/**
 * Converte un enum TournamentState in una stringa stampabile nella console.
 * @param state Lo stato del torneo (TournamentState)
 */
const char* cli_tournament_state_to_str(TournamentState state);

/**
 * Restituisce il messaggio corrispondente ad un enum ServiceStatus.
 * @param status Il valore di ServiceStatus per il quale ottenere il messaggio.
 */
const char* cli_service_status_to_str(ServiceStatus status);

/**
 * @brief Svuota la console per ripulire l'output che vede l'utente.
 * Utilizza comandi diversi a seconda del sistema operativo (Windows o Unix).
 */
void cli_clear_screen(void);

/**
 * @brief Laddove Mac OS e Linux (io, Roberto programmo su Mac OS) usano UTF-8 di default per il Terminale,
 * Windows no, usa uno standard obsoleto, che rompe tutte le lettere accentate, e in Italiano ne usiamo un bel pò...
 * Questa funzione si occupa di forzare UTF-8 su Windows per far apparire le lettere accentate correttamente.
 */
void cli_fix_windows_encoding(void);

#endif
