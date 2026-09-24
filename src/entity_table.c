#include "../include/entity_table.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Dato che la nostra EntityTable è fatta per contenere qualsiasi entità nel sistema, come puntatore all'inizio dei dati
 * dovrà usare un void* generico; tuttavia, così facendo, quando cerchiamo un elemento per ID non sapremo dove si troverà tale ID
 * all'interno dell'oggetto, dovremmo castarlo al tipo originale, ma non possiamo neanche sapere quale sarebbe. Quindi usiamo
 * un trucchetto: tutte le entità iniziano di proposito con un ID, sempre uint32_t, quindi usiamo questo struct generico
 * al quale castare ogni blocco di dati per poter estrapolare l'ID. Tale struct viene dichiarato solo qui nel file .c
 * perchè non vogliamo esporlo ad altri moduli nel sistema, deve essere usato solo qui */

/**
 * @brief Riservato per uso interno.
 */
typedef struct {
    uint32_t id;
} EntityHeader;

static ReadFileResult parse_file(EntityTable* table, FILE* file, int64_t magic_number) {
    int64_t parsed_magic = 0;
    if (fread(&parsed_magic, sizeof(parsed_magic), 1, file) != 1) {
        return READFILE_MISSING_MAGICNUMBER;
    }

    if (parsed_magic != magic_number) {
        return READFILE_INVALID_MAGICNUMBER;
    }

    if (fread(&table->count, sizeof(size_t), 1, file) != 1) {
        return READFILE_INVALID_COUNT;
    }

    if (fread(&table->next_id, sizeof(table->next_id), 1, file) != 1) {
        return READFILE_INVALID_NEXTID;
    }

    if (table->count > table->max_count) {
        table->count = table->max_count;
    }

    if (table->count + 5 > table->max_count) {
        table->capacity = table->max_count;
    } else {
        table->capacity = table->count + 5;
    }

    table->data = malloc(table->element_size * table->capacity);
    if (table->data == NULL) {
        return READFILE_OOM;
    }

    if (fread(table->data, table->element_size, table->count, file) != table->count) {
        free(table->data);
        table->data = NULL;
        return READFILE_CORRUPT_ARRAY;
    }

    return READFILE_SUCCESS;
}

static WriteFileResult write_file(const EntityTable* table, FILE* file, const int64_t magic_number) {
    // Scriviamo il magic number, il numero degli elementi nella tabella e il prossimo ID,
    // in ordine

    if (fwrite(&magic_number, sizeof(magic_number), 1, file) != 1) {
        return WRITEFILE_WRITEFAILURE;
    }

    if (fwrite(&table->count, sizeof(table->count), 1, file) != 1) {
        return WRITEFILE_WRITEFAILURE;
    }

    if (fwrite(&table->next_id, sizeof(table->next_id), 1, file) != 1) {
        return WRITEFILE_WRITEFAILURE;
    }

    // Scriviamo la tabella su file solo se c'è almeno un elemento
    if (table->count > 0) {
        if (fwrite(table->data, table->element_size, table->count, file) != table->count) {
            return WRITEFILE_WRITEFAILURE;
        }
    }

    return WRITEFILE_SUCCESS;
}

static WriteFileResult save_data(const EntityTable* table) {
    // Salta il salvataggio se la Batch mode è attiva!
    if (table->batch_mode) return WRITEFILE_SUCCESS;

    // Non salviamo sul file direttamente, ma su una copia; se il salvataggio avviene con successo,
    // rimuoviamo il file originale e rinominiamo questa copia temporanea. Questo riduce la
    // possibilità di perdite di dati

    char* tmp_filename = malloc(strlen(table->filename) + 5);
    if (tmp_filename == NULL) return WRITEFILE_OOM;

    strcpy(tmp_filename, table->filename);
    strcat(tmp_filename, ".tmp");

    WriteFileResult result;
    FILE* file = fopen(tmp_filename, "wb");

    if (file == NULL) {
        result = WRITEFILE_UNAUTHORIZED;
    } else {
        result = write_file(table, file, table->magic_number);
        fclose(file);

        if (result == WRITEFILE_SUCCESS) {
            remove(table->filename);
            rename(tmp_filename, table->filename);
        } else {
            remove(tmp_filename);
        }
    }

    free(tmp_filename);
    return result;
}

static ReadFileResult load_data(EntityTable* table) {
    table->count = 0;
    table->next_id = 1;
    table->capacity = 5;

    FILE* file = fopen(table->filename, "rb");

    if (file == NULL) {
        table->data = malloc(table->element_size * table->capacity);
        if (table->data == NULL) {
            return READFILE_OOM;
        }
        return READFILE_SUCCESS;
    }
    ReadFileResult result = parse_file(table, file, table->magic_number);
    fclose(file);

    return result;
}

size_t entity_table_add_item(EntityTable* table, void* new_item) {
    // Abbiamo raggiunto il limite massimo di elementi permesso da questa tabella. L'operazione fallisce
    if (table->count >= table->max_count) {
        return 0;
    }

    // Se la tabella è piena, dobbiamo allargarla
    if (table->count >= table->capacity) {
        table->capacity += 5;

        // Assegnamo questo puntatore ad una variabile d'appoggio prima per verificare che la
        // riallocazione sia avvenuta con successo
        void* tmp_ptr = realloc(table->data, table->element_size * table->capacity);

        if (tmp_ptr == NULL) {
            // La riallocazione è finita, il sistema ha finito la memoria;
            // Restituiamo 0, un ID impossibile dato che gli ID nel nostro
            // sistema partono da 1
            return 0;
        }
        table->data = tmp_ptr;
    }

    // Forziamo un cast ad EntityHeader* per settare l'ID
    EntityHeader* item_header = (EntityHeader*)new_item;
    item_header->id = table->next_id;

    // C non sa come spostarsi di indice in indice, avendo un puntatore a void;
    // castiamo a char* per operare con i byte

    // table->data punta all'inizio dei nostri dati;
    // table->count perchè vogliamo mettere l'elemento alla fine;
    // table->count * table->element_size perchè per calcolare l'offset al quale andare a scrivere,
    // partendo dall'inizio dei dati, calcoliamo numero di elementi PER la dimensione di ciascuno.

    // Per esempio, se abbiamo 5 elementi, ognuno con dimensione 10 byte, 5*10 = 50 byte,
    // e andiamo a scrivere il nostro nuovo elemento 50 byte dopo l'inizio

    char* dest = (char*)table->data + (table->count * table->element_size);
    memcpy(dest, new_item, table->element_size);

    table->count++;
    table->next_id++;

    WriteFileResult result = save_data(table);
    if (result != WRITEFILE_SUCCESS) {
        return 0;
    }

    // Restituiamo l'ID dell'elemento appena inserito
    return table->next_id - 1;
}

bool entity_table_edit_item(EntityTable* table, const uint32_t id, void* edited_item) {
    void* original_element = entity_table_get_item_by_id(table, id);
    // Non è associato alcun elemento all'ID passato! Non possiamo modificare qualcosa che non esiste.
    if (original_element == NULL) return false;

    // Nel caso in cui ci vengono passati dati spazzatura, dobbiamo almeno preservare e salvaguardare l'id dell'elemento
    // altrimenti la nostra tabella diventa un macello
    EntityHeader* edited_item_header = (EntityHeader*)edited_item;

    // Forziamo l'ID originale su edited_item. L'ID non si tocca!!
    edited_item_header->id = id;

    // Sovrascriviamo i dati dell'originale con edited_item
    memcpy(original_element, edited_item, table->element_size);

    WriteFileResult result = save_data(table);
    if (result != WRITEFILE_SUCCESS) {
        return false;
    }
    return true;
}

bool entity_table_delete_item(EntityTable* table, const uint32_t id) {
    void* element = entity_table_get_item_by_id(table, id);
    if (element == NULL) return false;

    // Calcoliamo l'indice nell'array dell'elemento usando aritmetica dei puntatori

    // Calcolo la distanza tra element e l'inizio della tabella, in byte.
    // Per esempio, se element è in posizione 4 (quindi quinta), ed ogni element è grande 10 byte, element sarà avanti di
    // 4*10 = 40 bytes rispetto all'inizio della tabella.
    size_t byte_offset = (char*)element - (char*)table->data;

    // Poi calcolo l'indice effettivo dividendo l'offset calcolato sopra (ovvero la distanza dall'inizio) per la dimensione dell'elemento.
    // Riprendendo l'esempio, con una distanza di 40 byte e una dimensione per elemento di 10 byte, 40 / 10 = 4. Abbiamo
    // ricavato l'indice dell'elemento!
    size_t index = byte_offset / table->element_size;

    // Calcoliamo il numero di elementi da spostare indietro, se l'elemento che stiamo eliminando si trovava in mezzo ad altri
    // Totale - indice dell'elemento - 1.
    // Se element era in posizione 4 ed abbiamo 8 elementi in totale, 8 - 4 - 1 = 3 elementi che erano dopo element che ora
    // dobbiamo rispostare indietro. Ci sta!
    size_t elements_to_move = table->count - index - 1;

    // Shiftiamo gli elementi indietro per chiudere lo spazio vuoto rimasto
    if (elements_to_move > 0) {
        // Sposteremo tutto a partire dalla posizione dell'elemento che stiamo eliminando
        void* dest = element;

        // Partendo da tutto quello che viene dopo
        void* src = (char*)element + table->element_size;

        memmove(dest, src, elements_to_move * table->element_size);
    }

    table->count--;

    // L'array ha più di 5 slot liberi - possiamo ridurre il suo spazio
    // a patto che l'array sia più grande della capacità minima, 5.
    if (table->count < table->capacity - 5 && table->capacity > 5) {
        size_t new_capacity = table->capacity - 5;
        void* tmp_ptr = realloc(table->data, new_capacity * table->element_size);

        if (tmp_ptr == NULL) {
            // ...praticamente impossibile. Rimpiccioliamo l'array e non c'è più memoria?!?
            // Nel dubbio, gestiamo anche questo caso assurdo. Possiamo salvare tanto la cancellazione è avvenuta comunque
            WriteFileResult result = save_data(table);
            if (result != WRITEFILE_SUCCESS) {
                return false;
            }
            return true;
        }

        table->capacity = new_capacity;
        table->data = tmp_ptr;
    }

    WriteFileResult result = save_data(table);
    if (result != WRITEFILE_SUCCESS) {
        return false;
    }
    return true;
}

void* entity_table_get_item_by_id(const EntityTable* table, const uint32_t id) {
    if (table == NULL || table->data == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < table->count; i++) {
        char* element_ptr = (char*)table->data + (i * table->element_size);
        EntityHeader* header = (EntityHeader*)element_ptr;

        if (header->id == id) {
            return element_ptr;
        }
    }
    return NULL;
}

void* entity_table_select(const EntityTable* table, const EntityFilter filter_function, const void* ctx, size_t* out_count) {
    if (table == NULL || table->data == NULL || out_count == NULL || table->count == 0) {
        // Non ci sono elementi!
        if (out_count != NULL) *out_count = 0;
        return NULL;
    }
    *out_count = 0;

    // Allochiamo il massimo possibile, ridimensioniamo dopo
    char* filtered_table = malloc(table->element_size * table->count);

    if (filtered_table == NULL) {
        // Impostare la conta a SIZE_MAX comunica agli altri layer del
        // sistema che abbiamo terminato la memoria disponibile
        *out_count = SIZE_MAX;
        return NULL;
    }

    for (size_t i = 0; i < table->count; i++) {
        // Il puntatore sarà data (il punto di partenza) + l'indice dell'elemento * la dimensione di un elemento
        // Castiamo a char* perchè aritmetica su puntatori void* può causare problemi con certi compilatori
        const void* element = (const char*)table->data + (i * table->element_size);
        if (filter_function == NULL || filter_function(element, ctx)) {
            memcpy(filtered_table + (*out_count * table->element_size), element, table->element_size);
            (*out_count)++;
        }
    }

    if (*out_count == 0) {
        // Ci sono elementi nella tabella, ma nessuno rientra nei criteri specificati.
        free(filtered_table);
        return NULL;
    }
    void* shrunk_table = realloc(filtered_table, table->element_size * *out_count);
    if (shrunk_table == NULL) {
        // Abbiamo finito la memoria... almeno abbiamo il puntatore originale stavolta. Passiamo quello
        return filtered_table;
    }
    return shrunk_table;
}

size_t entity_table_count(const EntityTable* table, const EntityFilter filter_function, const void* ctx) {
    if (table == NULL || table->data == NULL || table->count == 0) {
        // Non ci sono elementi!
        return 0;
    }
    size_t out_count = 0;
    for (size_t i = 0; i < table->count; i++) {
        // Il puntatore sarà data (il punto di partenza) + l'indice dell'elemento * la dimensione di un elemento
        // Castiamo a char* perchè aritmetica su puntatori void* può causare problemi con certi compilatori
        const void* element = (const char*)table->data + (i * table->element_size);
        if (filter_function == NULL || filter_function(element, ctx)) {
            out_count++;
        }
    }

    return out_count;
}

ReadFileResult init_entity_table(EntityTable* table, const size_t element_size, const size_t max_count, const int64_t magic_number, const char* filename) {
    table->element_size = element_size;
    table->max_count = max_count;
    table->magic_number = magic_number;
    table->filename = filename;
    table->batch_mode = false;
    table->data = NULL;

    ReadFileResult result = load_data(table);
    return result;
}

void entity_table_batch_begin(EntityTable* table) {
    table->batch_mode = true;
}

WriteFileResult entity_table_batch_commit(EntityTable* table) {
    table->batch_mode = false;
    return save_data(table);
}

WriteFileResult clear_table(EntityTable* table) {
    if (table == NULL) {
        return false;
    }

    // Assicuriamoci prima che possiamo allocare lo spazio per la tabella nuova ripulita
    void* new_data_ptr = malloc(table->element_size * 5);
    if (new_data_ptr == NULL) {
        // ...no, non possiamo! Per assurdo, non possiamo resettare la tabella. E' praticamente impossibile che finiremo
        // mai qui, ma meglio prevenire che curare
        return WRITEFILE_OOM;
    }

    // Ripristiniamo tutto
    table->next_id = 1;
    table->count = 0;
    table->capacity = 5;

    // Liberiamo la memoria precedente
    if (table->data != NULL) free(table->data);

    table->data = new_data_ptr;
    return save_data(table);
}


