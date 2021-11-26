#ifndef PROJECT_INCLUDE_CLIENTS_H_
#define PROJECT_INCLUDE_CLIENTS_H_

#define _GNU_SOURCE
#include <stdio.h>

typedef struct {
    int number;
    char *name;
    char *surname;
    char *address;
    char *telephone;
    double indebtedness;
    double credit_limit;
    double cash_payments;
} client_t;

int client_read(FILE *in, client_t *client);
int client_print(FILE *out, const client_t *client);
int clients_input_output_from_in_to_out(FILE *in, FILE *out);

#endif  // PROJECT_INCLUDE_CLIENTS_H_
