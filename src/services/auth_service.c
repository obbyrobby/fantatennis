#include "../../include/services/auth_service.h"
#include "../../include/models/user.h"
#include <string.h>

static bool filter_users(const void* element, const void* ctx) {
    const User* user = element;
    const bool include_admins = *(bool*)ctx;

    if (!user->is_admin) return true;
    return include_admins;
}

LoginResult try_login(AppContext* app_context, const char* username, const char* password) {
    User* users = (User*)app_context->users_table.data;

    for (size_t i = 0; i < app_context->users_table.count; i++) {
        if(strcmp(username, users[i].username) == 0) {
            if (strcmp(password, users[i].password) == 0) {
                app_context->current_user_id = users[i].id;
                app_context->is_admin = users[i].is_admin;
                return LOGIN_SUCCESS;
            }
            return LOGIN_WRONG_PASSWORD;
        }
    }
    return LOGIN_USERNAME_NOT_FOUND;
}

RegisterResult try_register(AppContext* app_context, const char* username, const char* password) {
    User* users = app_context->users_table.data;

    if (strlen(username) < MIN_USERNAME_LEN) {
        return REGISTER_USERNAME_TOO_SHORT;
    }

    if (strlen(password) < MIN_PASSWORD_LEN) {
        return REGISTER_PASSWORD_TOO_SHORT;
    }

    for (size_t i = 0; i < app_context->users_table.count; i++) {
        if (strcmp(username, users[i].username) == 0) {
            return REGISTER_USERNAME_ALREADY_TAKEN;
        }
    }

    User new_user = user_build(username, password, false);
    size_t user_id = entity_table_add_item(&app_context->users_table, &new_user);
    if (user_id == 0) {
        return REGISTER_FAILURE;
    }
    app_context->current_user_id = user_id;
    app_context->is_admin = false;

    return REGISTER_SUCCESS;
}

void logout(AppContext* app_context) {
    app_context->current_user_id = 0;
    app_context->is_admin = false;
}

bool get_user_by_id(const AppContext* app_context, const uint32_t id, User* user) {
    User* item = entity_table_get_item_by_id(&app_context->users_table, id);
    if (item == NULL) return false;
    if (user != NULL) *user = *item;
    return true;
}

User* get_users(const AppContext* app_context, const bool include_admins, size_t* users_count) {
    return entity_table_select(&app_context->users_table, filter_users, &include_admins, users_count);
}

ServiceStatus set_admin(AppContext* app_context, uint32_t user_id) {
    if (!app_context->is_admin) return STATUS_ERR_UNAUTHORIZED;
    User user;
    if (!get_user_by_id(app_context, user_id, &user)) {
        return STATUS_ERR_USER_NOT_FOUND;
    }
    user.is_admin = true;
    entity_table_edit_item(&app_context->users_table, user_id, &user);
    return STATUS_OK;
}