#include <stdlib.h>
#include <ctype.h>

#include "clients.h"
#include "errors.h"

static void skip_spaces(FILE *in) {
    char c = fgetc(in);

    while (isspace(c)) {
        c = fgetc(in);
    }

    if (isalpha(c))
        ungetc(c, in);
}

int client_read(FILE *in, client_t *client) {
    size_t n = 0;
    if (!in || !client)
        return INCORRECT_ARGS;

    if (fscanf(in, "%d", &client->number) != 1 && client->number <= 0)
        return INPUT_OUTPUT_ERROR;

    skip_spaces(in);

    int rc = getdelim(&client->name, &n, ' ', in);
    if (rc < 1)
        return INPUT_OUTPUT_ERROR;

    skip_spaces(in);

    n = 0;
    rc = getdelim(&client->surname, &n, ' ', in);
    if (rc < 1) {
        free(client->name);
        return INPUT_OUTPUT_ERROR;
    }

    skip_spaces(in);

    n = 0;
    rc = getdelim(&client->address, &n, ' ', in);
    if (rc < 1) {
        free(client->name);
        free(client->surname);
        return INPUT_OUTPUT_ERROR;
    }

    skip_spaces(in);

    n = 0;
    rc = getdelim(&client->telephone, &n, ' ', in);
    if (rc < 1) {
        free(client->name);
        free(client->surname);
        free(client->address);
        return INPUT_OUTPUT_ERROR;
    }

    if (fscanf(in, "%lf%lf%lf", &client->indebtedness, &client->credit_limit, &client->cash_payments) != 3
                    || client->indebtedness < 0 || client->credit_limit < 0 || client->cash_payments < 0) {
        return INPUT_OUTPUT_ERROR;
    }

    return OK;
}

int client_print(FILE *out, const client_t *client) {
    if (!out || !client)
        return INPUT_OUTPUT_ERROR;

    fprintf(out, "%-12d", client->number);
    fprintf(out, "%-11s", client->name);
    fprintf(out, "%-11s", client->surname);
    fprintf(out, "%-16s", client->address);
    fprintf(out, "%20s", client->telephone);
    fprintf(out, "%12.2f", client->indebtedness);
    fprintf(out, "%12.2f", client->credit_limit);
    fprintf(out, "%12.2f\n", client->cash_payments);

    return OK;
}

int clients_input_output_from_in_to_out(FILE *in, FILE *out) {
    if (!in || !out)
        return INCORRECT_ARGS;

    int rc = OK;
    client_t client = { 0 };
    do {
        puts("Enter information about client:\n"
             "1 Account number\n"
             "2 Client name\n"
             "3 Surname\n"
             "4 Client address\n"
             "5 Client telephone number\n"
             "6 Client indebtedness\n"
             "7 Client credit limit\n"
             "8 Client cash payments\n");
        rc = client_read(in, &client);
        if (rc) {
            break;
        }
        rc = client_print(out, &client);
        if (rc) {
            break;
        }
        free(client.name);
        free(client.surname);
        free(client.address);
        free(client.telephone);
    } while (rc == OK);

    if (!feof(in))
        rc = INPUT_OUTPUT_ERROR;

    return rc;
}
