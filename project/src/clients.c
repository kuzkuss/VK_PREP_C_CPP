#include <stdlib.h>
#include <ctype.h>

#include "clients.h"
#include "errors.h"

#define BUF_SIZE 1

static char *get_until_space(FILE *f_in);
static void skip_spaces(FILE *in);

static char *get_until_space(FILE *f_in) {
    int size = BUF_SIZE;
    int len = 0;
    char c;
    char *buf = malloc(size);
    while (!isspace(c = fgetc(f_in)) && !feof(f_in)) {
        size += BUF_SIZE;
        char *tmp = realloc(buf, size);
        if (!tmp) {
            free(buf);
            return NULL;
        }
        buf = tmp;
        buf[len] = c;
        len++;
    }
    buf[len] = '\0';
    return buf;
}

static void skip_spaces(FILE *in) {
    char c = fgetc(in);

    while (isspace(c)) {
        c = fgetc(in);
    }

    if (isalpha(c))
        ungetc(c, in);
}

int client_read(FILE *in, client_t *client) {
    if (!in || !client)
        return INCORRECT_ARGS;

    if (fscanf(in, "%d", &client->number) != 1 || client->number <= 0)
        return INPUT_OUTPUT_ERROR;

    skip_spaces(in);

    client->name = get_until_space(in);
    if (!client->name)
        return INPUT_OUTPUT_ERROR;

    skip_spaces(in);

    client->surname = get_until_space(in);
    if (!client->surname) {
        free(client->name);
        return INPUT_OUTPUT_ERROR;
    }

    skip_spaces(in);

    client->address = get_until_space(in);
    if (!client->address) {
        free(client->name);
        free(client->surname);
        return INPUT_OUTPUT_ERROR;
    }

    skip_spaces(in);

    client->telephone = get_until_space(in);
    if (!client->telephone) {
        free(client->name);
        free(client->surname);
        free(client->address);
        return INPUT_OUTPUT_ERROR;
    }

    if (fscanf(in, "%lf%lf%lf\n", &client->indebtedness, &client->credit_limit, &client->cash_payments) != 3
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

    if (feof(in))
        rc = OK;

    return rc;
}
