/**
 * @file entity_table.h
 * @brief Modulo per la EntityTable, una struttura usata per gestire in maniera automatica ed efficiente tutte
 * le collezioni di entità nel sistema
 * @author Roberto Massante e Martina Gisonda
 */

#ifndef FANTATENNIS_ENTITY_TABLE_H
#define FANTATENNIS_ENTITY_TABLE_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h> 

/**
 * @brief Possibili esiti di lettura da file di load_data()
 */
typedef enum {
    READFILE_SUCCESS,
    READFILE_MISSING_MAGICNUMBER,
    READFILE_INVALID_MAGICNUMBER,
    READFILE_INVALID_COUNT,
    READFILE_INVALID_NEXTID,
    READFILE_CORRUPT_ARRAY,
    READFILE_OOM
} ReadFileResult;

/**
 * @brief Possibili esiti di scrittura su file di save_data()
 */
typedef enum {
    WRITEFILE_SUCCESS,
    WRITEFILE_UNAUTHORIZED,
    WRITEFILE_WRITEFAILURE,
    WRITEFILE_OOM
} WriteFileResult;

/**
 * @brief Struttura usata per memorizzare in memoria tabelle di dati omogenei dinamiche. Usata per memorizzare ogni collezione di modelli nel sistema.
 *
 * @note Prima di qualsiasi operazione, deve essere istanziata con init_entity_table.
 * @warning <b>IMPORTANTE:
 *
 * 1) ASSOLUTAMENTE non modificare i dati e campi interni direttamente. Usare i metodi del modulo entity_table.h, entity_table_*() per qualsiasi operazione.
 * 2) Interagire con le EntityTable solo al livello Services, layer che interagiscono con l'utente o simili non devono ASSOLUTAMENTE interagirci direttamente.</b>
 *
 * @details Supporta riduzione/aumento dello spazio allocato in base agli elementi presenti, associazione automatica di un ID che
 * funge da chiave primaria per ogni entità, e shifting degli elementi quando un elemento nel mezzo dell'array viene
 * eliminato. Replica le funzionalità di base delle Liste in linguaggi OOP, o tabelle SQL.
 */
typedef struct {
    uint32_t next_id;       /**< L'ID da associare al prossimo elemento che verrà inserito nella lista. Non diminuisce mai, anche se vengono rimossi elementi, garantendo unicità */
    size_t count;           /**< La quantità di elementi attualmente presenti nella tabella */
    size_t capacity;        /**< La capacità della tabella. Da usare esclusivamente all'interno del modulo EntityTable per gestione interna */
    size_t element_size;    /**< La dimensione in byte di un singolo elemento. Da configurare immediatamente per corretto funzionamento della EntityTable */
    size_t max_count;       /**< Il numero massimo di elementi ammessi nella tabella */
    int64_t magic_number;   /**< Il magic number che verrà controllato all'inizio del file */
    bool batch_mode;        /**< Flag che indica se è attiva la modalità di Batching delle operazioni per la tabella */
    const char* filename;   /**< Il nome del file associato a questa tabella */
    void* data;             /**< Punta all'inizio dei dati della tabella */
} EntityTable;

/**
 * @brief Tipo di funzione usata con entity_table_select e entity_table_count per filtrare elementi da una tabella.
 * @details Le due funzioni chiameranno la funzione EntityFilter passata nei parametri ad ogni elemento nella tabella.
 * L'esito di EntityFilter detterà se l'elemento rientrerà nel risultato finale o meno.
 * @param element L'elemento da filtrare
 * @param ctx Ulteriore parametro da passare, se necessario, per fornire ulteriore contesto al filtro.
 * @return true se l'elemento appartiene alla tabella di output, altrimenti false
 */
typedef bool (*EntityFilter)(const void* element, const void* ctx);

/**
 * Aggiunge un elemento ad una EntityTable.
 * @param table Il puntatore alla EntityTable dove aggiungere l'elemento
 * @param new_item Il puntatore all'elemento da inserire. I dati presenti all'indirizzo passato saranno copiati in tabella;
 * non verrà salvato il puntatore.
 * @note L'elemento associato al puntatore verrà modificato per ricevere l'ID appena assegnato in tabella.
 * @return L'ID dell'elemento appena aggiunto. Sarà 0 se l'inserimento è fallito a causa di mancanza di memoria
 */
size_t entity_table_add_item(EntityTable* table, void* new_item);

/**
 * Modifica un elemento in una EntityTable.
 * @param table Il puntatore alla EntityTable dove modificare l'elemento
 * @param id L'ID dell'elemento da modificare (non l'indice nell'array!)
 * @param edited_item Il puntatore all'elemento con i nuovi dati inseriti, che saranno clonati all'interno della tabella
 * @return L'esito dell'operazione: true se tutto ok, false se l'id passato non è associato ad alcun elemento nella tabella
 * @note Usare questa funzione per modificare dati all'interno della EntityTable anzichè modificarli direttamente.
 * Ciò consente una corretta astrazione e divisione degli scopi, e garantisce integrità dei dati.
 */
bool entity_table_edit_item(EntityTable* table, uint32_t id, void* edited_item);

/**
 * Cancella un elemento da una EntityTable.
 * @param table Il puntatore alla EntityTable dalla quale rimuovere l'elemento
 * @param id L'id dell'elemento da rimuovere
 * @return L'esito dell'operazione: true se tutto ok, false se l'id passato non è associato ad alcun elemento nella tabella
 */
bool entity_table_delete_item(EntityTable* table, uint32_t id);

/**
 * Restituisce l'elemento in tabella associato ad un determinato ID. <b>(NON l'indice/posizione dell'elemento nella tabella!)</b>
 * @param table Il puntatore alla EntityTable dove cercare l'elemento
 * @param id L'ID dell'elemento da cercare
 * @return Il puntatore all'elemento ricercato. Sarà NULL se non c'è alcun elemento associato all'ID richiesto
 */
void* entity_table_get_item_by_id(const EntityTable* table, uint32_t id);

/**
 * Funzione che restituisce copie di dati da una EntityTable.
 * @warning L'array dei risultati è allocato dinamicamente; è OBBLIGATORIO rilasciare la memoria
 * usando free() sul valore restituito da questo metodo per prevenire un memory leak!
 * @param table Il puntatore alla EntityTable dalla quale estrapolare i dati
 * @param filter_function OPZIONALE: una funzione di tipo EntityFilter che filtra i risultati
 * @param ctx OPZIONALE: Un puntatore a dati che forniscono ulteriore contesto alla funzione filtro, ove necessario
 * @param out_count Il puntatore alla variabile che conterrà il numero di elementi restituiti.
 * @return Il puntatore all'inizio della copia dei risultati
 */
void* entity_table_select(const EntityTable* table, EntityFilter filter_function, const void* ctx, size_t* out_count);

/**
 * Funzione che restituisce la quantità di elementi in una EntityTable.
 * @param table Il puntatore alla EntityTable nella quale contare i dati
 * @param filter_function OPZIONALE: una funzione di tipo EntityFilter che filtra i risultati ed esclude elementi dalla conta
 * @param ctx OPZIONALE: Un puntatore a dati che forniscono ulteriore contesto alla funzione filtro, ove necessario
 * @return La quantità di elementi nella EntityTable che rientrano nei filtri specificati.
 */
size_t entity_table_count(const EntityTable* table, EntityFilter filter_function, const void* ctx);

/**
 * Inizializza una EntityTable
 * @param table Il puntatore alla EntityTable da inizializzare
 * @param element_size La dimensione in byte di un singolo elemento
 * @param max_count Il numero massimo di elementi ammessi nella tabella
 * @param magic_number Il magic number che verrà controllato all'inizio del file
 * @param filename Il nome del file associato a questa tabella
 */
ReadFileResult init_entity_table(EntityTable* table, size_t element_size, size_t max_count, int64_t magic_number, const char* filename);

/**
 * Attiva la modalità di Batching delle operazioni per la tabella specificata.
 * @details Ogni modifica ai dati della tabella non sarà più salvata immediatamente su file, ma avverrà solo
 * nella memoria del processo. Una volta terminate tutte le operazioni del caso, chiamare entity_table_batch_commit()
 * per terminare la modalità di Batching e salvare tutte le modifiche su file in un colpo solo. Questo permette di effettuare
 * molteplici operazioni sulla EntityTable senza che venga aperto il suo file e vengano riscritti tutti i dati da capo per ogni singola modifica.
 * @see entity_table_batch_commit
 * @param table Il puntatore alla tabella per la quale attivare la modalità di Batching
 */
void entity_table_batch_begin(EntityTable* table);

/**
 * Termina la modalità di Batching delle operazioni per la tabella specificata.
 * @details Termina la modalità, e salva su disco tutti le modifiche fatte alla tabella dall'inizio della modalità di Batching.
 * @param table Il puntatore alla tabella per la quale terminare la modalità di Batching e salvare tutte le modifiche
 * @return L'esito di scrittura del file
 */
WriteFileResult entity_table_batch_commit(EntityTable* table);

/**
 * Azzera una EntityTable, cancellando tutti i dati al suo interno.
 * @warning Funzione pericolosa!! Usare con cautela. L'operazione è irreversibile
 * @param table Il puntatore alla tabella da azzerare
 * @result L'esito del reset e scrittura del file
 */
WriteFileResult clear_table(EntityTable* table);

#endif
