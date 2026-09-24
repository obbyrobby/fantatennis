#include "../../include/utils/string_utils.h"
#include <ctype.h>

bool str_contains(const char* str, const char* substr) {
    if (substr[0] == '\0') {
        // Una stringa tecnicamente contiene sempre il vuoto!
        return true;
    }

    // Iteriamo fino a quando non arriviamo al NULL terminator
    for (int i = 0; str[i] != '\0'; i++) {
        int str_index = i;
        int substr_index = 0;

        // Sub-iterazione, partendo da ogni carattere della stringa principale
        // Il controllo è case-insensitive!
        while (str[str_index] != '\0' && substr[substr_index] != '\0' &&
            tolower((unsigned char)str[str_index]) == tolower((unsigned char)substr[substr_index])) {

            // Se i caratteri di str e substr corrispondono, andiamo avanti di un carattere per entrambe
            str_index++;
            substr_index++;
        }

        if (substr[substr_index] == '\0') {
            // substr è arrivata alla fine - significa che rientra tutta in str!
            return true;
        }
    }

    return false;
}
