#include "../../include/cli/cli_startuperr.h"
#include <stdio.h>

void handle_startuperr(const ReadFileResult result, char* bad_file)
{
    printf("[x] E' avvenuto un problema durante la fase di inizializzazione dell'applicazione. \nCAUSA: ");
    switch (result) {
        case READFILE_MISSING_MAGICNUMBER:
            printf("Il magic number è assente nel file %s.", bad_file);
            break;
        case READFILE_INVALID_MAGICNUMBER:
            printf("Il magic number è invalido nel file %s", bad_file);
            break;
        case READFILE_INVALID_COUNT:
            printf("La quantita' di elementi è assente nel file %s.", bad_file);
            break;
        case READFILE_CORRUPT_ARRAY:
            printf("L'array di dati nel file %s è corrotto.", bad_file);
            break;
        case READFILE_INVALID_NEXTID:
            printf("Il parametro nextID nel file %s è corrotto.", bad_file);
            break;
        case READFILE_OOM:
            printf("Non c'è abbastanza memoria disponibile nel computer per caricare il file %s. "
                   "Il file potrebbe essere ecessivamente grande; prova a chiudere applicazioni non in uso e riprova.", bad_file);
            break;
        case READFILE_SUCCESS:
            printf("...come siamo arrivati qui?! file: %s", bad_file);
            break;
    }
    printf("\n\nElimina o ripara il file problematico, e riprova ad avviare l'applicazione.");
}
