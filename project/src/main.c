#include <stdio.h>

#include "errors.h"
#include "menu.h"

int main(void) {
    int rc = client_base_processing();

    if (rc == FILE_ERROR) {
        puts("File error.\n");
    } else if (rc == INPUT_OUTPUT_ERROR) {
        puts("Input-output error.\n");
    } else if (rc == INCORRECT_ARGS) {
        puts("Incorrect arguments of function.\n");
    }

    return rc;
}

