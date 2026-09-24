#include <stdio.h>
#include "../../include/cli/cli_login.h"
#include "../../include/config.h"
#include "../../include/cli/cli_utils.h"
#include "../../include/services/auth_service.h"

bool cli_login(AppContext* app_context) {
    cli_clear_screen();
    char username_buf[USERNAME_SIZE] = { 0 };
    char password_buf[PASSWORD_SIZE] = { 0 };

    printf("\nBenvenuto al sistema di Fantatennis!\n\n");
    while (true) {
        printf("Hai già un account? ");
        int choice;
        cli_pick_option((const char*[]){
            "Si, vorrei loggare nel mio account", "No, vorrei registrare un nuovo account", "Esci dall'applicazione"
        }, 3, &choice, false);

        if (choice == 0) {
            // Esci dal loop automaticamente se l'utente ha una sessione valida
            printf("\n\n=== Login ===\n");
            while (app_context->current_user_id == 0) {
                printf("\nInserisci il tuo username (o lascia vuoto per annullare): ");
                if (!cli_read_string(username_buf, sizeof(username_buf))) {
                    break;
                }

                printf("\nInserisci la tua password: ");
                cli_read_string(password_buf, sizeof(password_buf));

                LoginResult result = try_login(app_context, username_buf, password_buf);
                switch (result) {
                    case LOGIN_SUCCESS:
                        printf("\nLogin avvenuto con successo! Benvenuto/a, %s", username_buf);
                        return true;
                    case LOGIN_USERNAME_NOT_FOUND:
                        printf("\nL'username %s non esiste nel sistema.\n", username_buf);
                        break;
                    case LOGIN_WRONG_PASSWORD:
                        printf("\nLa password per l'username %s è errata.\n", username_buf);
                        break;
                }
            }
        }

        if (choice == 1) {
            // Esci dal loop automaticamente se l'utente ha una sessione valida
            while (app_context->current_user_id == 0) {
                printf("\n=== Registrazione ===\n\nInserisci il tuo nuovo username (o lascia vuoto per annullare): ");
                if (!cli_read_string(username_buf, sizeof(username_buf))) {
                    break;
                }

                printf("\nInserisci la password per il tuo nuovo account: ");
                cli_read_string(password_buf, sizeof(password_buf));

                RegisterResult result = try_register(app_context, username_buf, password_buf);
                switch(result) {
                    case REGISTER_SUCCESS:
                        printf("\n\nRegistrazione avvenuta con successo! Benvenuto/a, %s", username_buf);
                        return true;
                    case REGISTER_USERNAME_ALREADY_TAKEN:
                        printf("\n\nL'username %s è già in uso.\n\n", username_buf);
                        break;
                    case REGISTER_FAILURE:
                        printf("\n\nE' avvenuto un errore durante la creazione dell'account.\nIl sistema potrebbe aver terminato "
                               "la memoria disponibile, o non è in grado di scrivere su file.\n\n");
                        break;
                    case REGISTER_USERNAME_TOO_SHORT:
                        printf("\nL'username è troppo corto! Deve essere almeno lungo %d caratteri.\n", MIN_USERNAME_LEN);
                        break;
                    case REGISTER_PASSWORD_TOO_SHORT:
                        printf("\nLa password è troppo corta! Deve essere almeno lunga %d caratteri.\n", MIN_PASSWORD_LEN);
                        break;
                }
            }
        }

        if (choice == 2) return false;
    }

}