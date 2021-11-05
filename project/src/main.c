#include <stdlib.h>
#include "email_parser.h"

int main(int argc, const char **argv) {
    if (argc != 2) {
        return -1;
    }

    const char *path_to_eml = argv[1];

    FILE *f = fopen(path_to_eml, "r");
    if (!f)
        return ERROR;


    char *from = NULL;
    char *to = NULL;
    char *date = NULL;
    int parts = 0;
    if (parse(f, &from, &to, &date, &parts) != OK)
        return ERROR;

    printf("%s|%s|%s|%d", from, to, date, parts);

    free(from);
    free(to);
    free(date);

    fclose(f);

    return OK;
}

