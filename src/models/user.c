#include "../../include/models/user.h"

#include <stdio.h>
#include <string.h>

User user_build(const char* username, const char* password, const bool is_admin) {
    // L'ID corretto sarà inserito dai meccanismi della entity_table - gli diamo giusto un valore temporaneo
    // così che l'IDE non rompe le scatole con valori non inizializzati
    User new_user = {0};

    snprintf(new_user.username, sizeof(new_user.username), "%s", username);
    snprintf(new_user.password, sizeof(new_user.password), "%s", password);
    new_user.is_admin = is_admin;

    return new_user;
}
