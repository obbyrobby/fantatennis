#include "../../include/cli/cli_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void flush_stdin(void) {
    // Quando chiamiamo fgets su stdin e leggiamo solo un numero fisso di caratteri, i caratteri
    // che avanzano (se presenti) rimarranno nello stream. Alla prossima chiamata di fgets, i caratteri rimasti
    // dall'input precedente saranno consumati e all'utente non verrà fatta un'altra richiesta di input!
    // Quindi con questa funzione andiamo a pulire lo stream stdin per evitare questa situazione
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

bool cli_read_int(const int min, const int max, int* value, bool allow_cancel, bool show_message) {
    while (true) {
        if (show_message) {
            if (allow_cancel) {
                printf("\nInserire un numero tra %d e %d (o lasciare vuoto per annullare): ", min, max);
            } else {
                if (max - min <= 1) {
                    printf("\nInserisci %d o %d: ", min, max);
                } else {
                    printf("\nInserire un numero tra %d e %d: ", min, max);
                }
            }
        }

        char buffer[30];
        if (!cli_read_string(buffer, sizeof(buffer))) {
            if (allow_cancel) {
                return false;
            }
            printf("\nL'input non può essere vuoto, riprova.\n");
            continue;
        }

        char* cast_ptr;
        // strtol leggerà la stringa carattere per carattere. Per ogni cifra che trova, la mette in val
        // e avanza il puntatore cast_ptr di un byte. Se trova un carattere che non è una cifra, si ferma.
        // se cast_ptr è uguale a buffer, significa che non si è mosso di un centimetro dal puntatore di inizio...
        // il che ci dice che l'input è invalido
        long val = strtol(buffer, &cast_ptr, 10);

        if (cast_ptr == buffer) {
            printf("\nInput invalido, riprova.\n");
            continue;
        }

        if(val < min || val > max) {
            printf("\nValore al di fuori del range, riprova.\n");
            continue;
        }

        (*value) = (int)val;
        return true;
    }
}

bool cli_read_string(char* str, const int len) {
    if (!fgets(str, len, stdin)) {
        return false;
    }

    if(!strchr(str, '\n')) {
        flush_stdin();
    } else {
        // Rimuoviamo \n dalla stringa
        str[strcspn(str, "\n")] = 0;
    }

    if (strlen(str) == 0) {
        return false;
    }
    return true;
}

bool cli_pick_option(const char** options, const int count, int* choice, bool allow_cancel) {
    printf("\n");
    for (int i = 0; i < count; i++) {
        printf("%d) %s\n", i+1, options[i]);
    }

    bool is_valid = cli_read_int(1, count, choice, allow_cancel, true);
    if (!is_valid) {
        return false;
    }

    // All'utente presentiamo le opzioni da 1 a count;
    // Gli indici interni però vanno da 0 a count-1, quindi una volta letto
    // il valore in input, sottraiamo 1 per farlo corrispondere
    (*choice)--;

    return true;
}

void cli_run_menu(AppContext* app_context, const MenuOption* options, const int count, const char* quit_caption, void* data) {
    // Allochiamo spazio per un numero variabile di puntatori a stringhe (non per stringhe e basta!)
    // per passare le descrizioni a cli_pick_option dopo

    // count + 1 per fare spazio all'opzione di uscita
    const char** option_names = malloc(sizeof(char*) * (count + 1));

    // Copiamo i puntatori
    for (int i = 0; i < count; i++) {
        option_names[i] = options[i].description;
    }
    option_names[count] = quit_caption;

    while (true) {
        if (app_context->current_user_id == 0) {
            // Abbiamo una sessione invalida, usciamo dal loop
            break;
        }
        int choice;
        cli_pick_option(option_names, count+1, &choice, false);

        if (choice == count) {
            // L'utente ha selezionato l'opzione di uscita, usciamo dal loop
            break;
        }
        options[choice].action(app_context, data);
    }

    free(option_names);
}

const char* cli_tournament_state_to_str(const TournamentState state) {
    switch (state) {
        case TOURNAMENT_STATE_OPEN:
            return "Aperto";
            break;
        case TOURNAMENT_STATE_ONGOING:
            return "In corso";
            break;
        case TOURNAMENT_STATE_COMPLETED:
            return "Terminato";
            break;
        default:
            return "???";
            break;
    }
}

const char* cli_service_status_to_str(ServiceStatus status) {
    switch (status) {
        case STATUS_OK:
            return "Operazione avvenuta con successo!";
        case STATUS_ERR_UNAUTHORIZED:
            return "Non hai i privilegi necessari per eseguire questa operazione.";
        case STATUS_ERR_NOT_FOUND:
            return "L'ID specificato è invalido.";
        case STATUS_ERR_OOM:
            return "Il sistema ha esaurito la memoria disponibile. Impossibile eseguire questa operazione.";
        case STATUS_ERR_PLAYER_BUSY:
            return "Questo tennista sta attualmente giocando in uno o più tornei in corso, quindi la modifica dei suoi dati non è consentita. "
                   "Sarà riammessa solo una volta tutti i tornei in cui è coinvolto/a saranno terminati.";
        case STATUS_ERR_PLAYER_HAS_HISTORY:
            return "Questo tennista è partecipante di uno o più tornei nel sistema, e di conseguenza\nnon può essere eliminato/a al fine di "
                   "preservare i risultati dei tornei e la loro integrità.\nSi consiglia invece di impostarlo/a come Non Disponibile.\nL'unico "
                   "modo per eliminare questo/a Tennista è eliminare dal sistema tutti i Tornei ai quali ha partecipato.";
        case STATUS_ERR_NAME_EMPTY:
            return "Il nome del Tennista non può essere vuoto!";
        case STATUS_ERR_SURNAME_EMPTY:
            return "Il cognome del Tennista non può essere vuoto!";
        case STATUS_ERR_TITLE_EMPTY:
            return "Il titolo del Torneo non può essere vuoto!";
        case STATUS_ERR_NOT_ENOUGH_PLAYERS:
            return "Il torneo non può essere impostato come 'In Corso' se non ci sono almeno due tennisti partecipanti.";
        case STATUS_ERR_TOURNAMENT_ALREADY_STARTED:
            return "Il torneo non può essere riaperto se è stata già giocata una partita o più.";
        case STATUS_ERR_TOURNAMENT_NOT_STARTED:
            return "Il torneo non può essere impostato come Completato se non è stata giocata neanche una partita.";
        case STATUS_ERR_ILLEGAL_TRANSITION:
            return "Questo passaggio di stato non è ammesso.";
        case STATUS_ERR_PLAYER_NOT_FOUND:
            return "L'ID specificato non appartiene ad alcun Tennista nel sistema.";
        case STATUS_ERR_TOURNAMENT_NOT_FOUND:
            return "L'ID specificato non appartiene ad alcun Torneo nel sistema.";
        case STATUS_ERR_PLAYER_UNAVAILABLE:
            return "Il tennista specificato è segnato come non Disponibile. Impossibile aggiungerlo.";
        case STATUS_ERR_PLAYER_ALREADY_PARTICIPATING:
            return "Il Tennista specificato partecipa già al Torneo.";
        case STATUS_ERR_WINNER_NOT_SET:
            return "Il torneo non può essere impostato come Completato se non viene anche specificato un vincitore.";
        case STATUS_ERR_INVALID_WINNER:
            return "Il vincitore specificato non è un partecipante del torneo.";
        case STATUS_ERR_RANKING_NOT_SET:
            return "Il ranking del Tennista non è stato impostato!";
        case STATUS_ERR_TIE_NOT_ALLOWED:
            return "Il pareggio non è ammesso. I set vinti non possono essere uguali.";
        case STATUS_ERR_INVALID_PLAYERS:
            return "I giocatori specificati non appartengono al torneo.";
        case STATUS_ERR_TOURNAMENT_NOT_ONGOING:
            return "Il torneo non è in corso. Le partite possono essere modificate solo in tornei in corso.";
        case STATUS_ERR_SAME_PLAYERS:
            return "Un tennista non può giocare contro se stesso!";
        case STATUS_ERR_USER_ALREADY_SIGNED_UP:
            return "Sei già iscritto/a a questo torneo!";
        case STATUS_ERR_TOURNAMENT_NOT_OPEN:
            return "Il torneo non è aperto. Le iscrizioni sono chiuse.";
        case STATUS_ERR_PLAYER_NOT_IN_TOURNAMENT:
            return "L'ID specificato non corrisponde ad un tennista nel torneo.";
        case STATUS_ERR_TEAM_NOT_FILLED:
            return "La squadra deve contenere 6 tennisti!";
        case STATUS_ERR_BUDGET_EXCEEDED:
            return "Hai superato il budget massimo di 100 crediti per comporre una squadra! Questa squadra non è ammessa.";
        case STATUS_ERR_USER_NOT_SIGNED_UP:
            return "Non sei ancora iscritto a questo torneo! Devi iscriverti prima di poter creare una squadra.";
        case STATUS_ERR_PLAYER_DUPED:
            return "E' stato inserito lo stesso Tennista due o più volte!";
        case STATUS_ERR_USER_NOT_FOUND:
            return "L'ID inserito non corrisponde ad alcun utente nel sistema.";
    }
    return "E' avvenuto un errore.";
}

void cli_clear_screen(void) {
    // Uso di preprocessore per la portabilità tra sistemi operativi
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void cli_fix_windows_encoding(void) {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif
}