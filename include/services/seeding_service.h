/**
 * @file seeding_service.h
 * @brief Modulo del servizio di Seeding
 * @author Roberto Massante e Martina Gisonda
*/

#ifndef FANTATENNIS_SEEDING_SERVICE_H
#define FANTATENNIS_SEEDING_SERVICE_H
#include "../app_context.h"

/**
 * Funzione di Seeding per riempire tutte le tabelle nel sistema con dati di esempio, al fine di poter testare ogni
 * funzione del programma
 * @warning Questa funzione azzera TUTTE LE TABELLE nel sistema prima di inserire i dati di esempio!!
 * E' necessario al fine di evitare di inserire dati duplicati, o incoerenti, con foreign key che puntano ad ID sbagliati.
 * Usare con molta cautela.
 * @param app_context Il puntatore al contesto dell'applicazione
 */
void seed_system(AppContext* app_context);

#endif
