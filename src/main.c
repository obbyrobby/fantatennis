#include <stdio.h>

#include "../include/app_context.h"
#include "../include/cli/cli_login.h"
#include "../include/cli/cli_startuperr.h"
#include "../include/cli/cli_mainmenu.h"
#include "../include/cli/cli_utils.h"


int main(void) {
    cli_fix_windows_encoding();
    AppContext app_context;
    char bad_file[50] = "";

    ReadFileResult result = init_app_context(&app_context, bad_file);
    if(result != READFILE_SUCCESS) {
        handle_startuperr(result, bad_file);
        return EXIT_FAILURE;
    }

    while (true) {
        if (!cli_login(&app_context)) {
            // L'utente ha selezionato Esci
            break;
        }
        if (app_context.is_admin) {
            cli_adminmenu(&app_context);
        } else {
            cli_regularmenu(&app_context);
        }
    }

    destroy_app_context(&app_context);
    return EXIT_SUCCESS;
}
