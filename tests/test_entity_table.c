/**
 * @file test_entity_table.c
 * @brief Modulo di Test per la EntityTable
 * @author Roberto Massante e Martina Gisonda
 */

#include <stdio.h>
#include <string.h>
#include "unity.h"
#include "../include/entity_table.h"

// Piccola struttura per testare la EntityTable con un'entità generica.
// Come ogni altra entità, deve iniziare con un uint32_t id.
typedef struct {
    uint32_t id;
    int value;
} TestItem;

static const char TEST_FILE[] = "entity_table_test.bin";
static const int64_t TEST_MAGIC_NUMBER = 0x1234567890ABCDELL;
static const size_t MAX_ITEMS = 10;

static EntityTable table;

// Usato come filtro di test
static bool filter_even_values(const void* element, const void* ctx) {
    (void)ctx;
    const TestItem* item = element;
    return item->value % 2 == 0;
}

// Eseguito prima di ogni unit test
void setUp(void) {
    remove(TEST_FILE);
    ReadFileResult result = init_entity_table(&table, sizeof(TestItem), MAX_ITEMS, TEST_MAGIC_NUMBER, TEST_FILE);
    TEST_ASSERT_EQUAL(READFILE_SUCCESS, result);
}

// Eseguito dopo ogni unit test
void tearDown(void) {
    free(table.data);
    table.data = NULL;
    remove(TEST_FILE);
}

// Test n° 1: Testa la creazione di un elemento nella EntityTable, e verifica che i suoi dati vengano memorizzati correttamente
static void test_additem_getbyid(void) {
    TestItem item = { 0, 29052007 };
    size_t id = entity_table_add_item(&table, &item);

    // L'ID è corretto?
    TEST_ASSERT_EQUAL(1, id);

    // La quantità di elementi è quella che ci aspettiamo?
    TEST_ASSERT_EQUAL(1, entity_table_count(&table, NULL, NULL));

    TestItem* found = entity_table_get_item_by_id(&table, id);
    // L'oggetto che abbiamo appena inserito è stato trovato?
    TEST_ASSERT_NOT_NULL(found);

    // Il valore è rimasto quello?
    TEST_ASSERT_EQUAL(29052007, found->value);
    TEST_ASSERT_EQUAL(1, found->id);

    // Se facciamo ricerche a caso, otteniamo NULL?
    TEST_ASSERT_NULL(entity_table_get_item_by_id(&table, 99));
}

// Test n° 2: Testa la modifica di un elemento nella EntityTable, e che le modifiche persistano nel sistema
static void test_edititem(void) {
    // Inseriamo un oggetto
    TestItem item = { 0, 10 };
    TEST_ASSERT_EQUAL(1, entity_table_add_item(&table, &item));

    // Modifichiamo l'ID a caso - dovrà essere ignorato dato che stiamo passando l'ID come parametro
    TestItem edited = { 999, 20 };
    TEST_ASSERT_TRUE(entity_table_edit_item(&table, 1, &edited));

    // Vediamo se ritroviamo l'oggetto modificato
    TestItem* found = entity_table_get_item_by_id(&table, 1);
    TEST_ASSERT_NOT_NULL(found);

    // Il valore è stato aggiornato?
    TEST_ASSERT_EQUAL(20, found->value);
    TEST_ASSERT_EQUAL(1, found->id);

    // Se cerchiamo un elemento non esistente, otteniamo false?
    TEST_ASSERT_FALSE(entity_table_edit_item(&table, 123, &edited));
}

// Test n° 3: Testa la cancellazione di un elemento nella EntityTable, che avvenga con successo e che
// gli altri elementi siano completamente illesi
static void test_deleteitem(void) {
    // Aggiungiamo 3 oggetti
    for (int i = 1; i <= 3; i++) {
        TestItem item = { 0, i * 10 };
        TEST_ASSERT_EQUAL(i, entity_table_add_item(&table, &item));
    }

    // E' stato eliminato il n. 2 con successo?
    TEST_ASSERT_TRUE(entity_table_delete_item(&table, 2));

    // La quantità di elementi è scesa a 2?
    TEST_ASSERT_EQUAL(2, entity_table_count(&table, NULL, NULL));

    // Non possiamo più trovare l'elemento appena eliminato, no?
    TEST_ASSERT_NULL(entity_table_get_item_by_id(&table, 2));

    // Il primo e il terzo oggetto sono ancora lì ed hanno i valori corretti, giusto?
    TestItem* first = entity_table_get_item_by_id(&table, 1);
    TestItem* third = entity_table_get_item_by_id(&table, 3);
    TEST_ASSERT_NOT_NULL(first);
    TEST_ASSERT_NOT_NULL(third);
    TEST_ASSERT_EQUAL(10, first->value);
    TEST_ASSERT_EQUAL(30, third->value);

    // Eliminare un elemento che non esiste restituisce false, giusto?
    TEST_ASSERT_FALSE(entity_table_delete_item(&table, 42));
}

// Test n° 4: Testa la selezione di elementi dalla EntityTable con una funzione filtro
static void test_selectwithfilter(void) {
    // Aggiungiamo 4 elementi
    TestItem items[4] = {
        {0, 11},
        {0, 22},
        {0, 33},
        {0, 44}
    };
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_NOT_EQUAL(0, entity_table_add_item(&table, &items[i]));
    }

    // Proviamo a selezionare solo gli oggetti con valore pari
    size_t count = 0;
    TestItem* results = entity_table_select(&table, filter_even_values, NULL, &count);
    TEST_ASSERT_NOT_NULL(results);
    TEST_ASSERT_EQUAL(2, count);

    // Abbiamo ottenuto una risposta corretta? Solo 22 e 44
    TEST_ASSERT_EQUAL(22, results[0].value);
    TEST_ASSERT_EQUAL(44, results[1].value);
    free(results);

    // Abbiamo 4 oggetti in totale, solo 2 quando contiamo esclusivamente quelli pari?
    TEST_ASSERT_EQUAL(4, entity_table_count(&table, NULL, NULL));
    TEST_ASSERT_EQUAL(2, entity_table_count(&table, filter_even_values, NULL));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_additem_getbyid);
    RUN_TEST(test_edititem);
    RUN_TEST(test_deleteitem);
    RUN_TEST(test_selectwithfilter);

    return UNITY_END();
}
