cmake_minimum_required(VERSION 4.3)
project(Fantatennis_Tests C)

# ---------------------------------------------------------
# Configurazione Framework di Test (Unity)
# ---------------------------------------------------------
add_library(unity_framework STATIC
        unity/unity.c
        unity/unity.h
        unity/unity_internals.h
)

# Rendiamo le directory di unity accessibili a chiunque linki questa libreria
target_include_directories(unity_framework PUBLIC
        unity
)

# ---------------------------------------------------------
# Definizione Include del Progetto
# ---------------------------------------------------------
# Definiamo i percorsi per includere correttamente i file .h del progetto principale
set(APP_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

# ---------------------------------------------------------
# Raccolta Sorgenti Applicazione
# ---------------------------------------------------------
# Recuperiamo tutti i file .c del progetto (escluso main.c per evitare conflitti di entry-point)
file(GLOB_RECURSE ALL_APP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.c")
list(FILTER ALL_APP_SOURCES EXCLUDE REGEX ".*/main\\.c$")

# ---------------------------------------------------------
# Target: test_entity_table
# ---------------------------------------------------------
add_executable(test_entity_table
        test_entity_table.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/entity_table.c
)

target_include_directories(test_entity_table PRIVATE
        ${APP_INCLUDES}
)

target_link_libraries(test_entity_table PRIVATE
        unity_framework
)

# Definiamo UNIT_TESTS per attivare i file di configurazione di test in config.h
# Usiamo PRIVATE per evitare che il flag passi accidentalmente ad altri target
target_compile_definitions(test_entity_table PRIVATE
        UNIT_TESTS
        UNITY_INCLUDE_DOUBLE
)

# ---------------------------------------------------------
# Target: test_services
# ---------------------------------------------------------
# Per i test dei servizi, abbiamo bisogno di tutto il codice sorgente dell'app
add_executable(test_services
        test_services.c
        ${ALL_APP_SOURCES}
        test_services_data.h
)

target_include_directories(test_services PRIVATE
        ${APP_INCLUDES}
)

target_link_libraries(test_services PRIVATE
        unity_framework
)

# Definiamo UNIT_TESTS per i servizi e i flag per Unity
target_compile_definitions(unity_framework PUBLIC
        UNITY_INCLUDE_DOUBLE
)

target_compile_definitions(test_services PRIVATE
        UNIT_TESTS
)


# Registriamo i nomi dei test per poterli lanciare con 'ctest'
add_test(NAME EntityTableTests COMMAND test_entity_table)
add_test(NAME ServiceTests COMMAND test_services)
