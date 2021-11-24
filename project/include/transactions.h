#ifndef PROJECT_INCLUDE_TRANSACTIONS_H_
#define PROJECT_INCLUDE_TRANSACTIONS_H_

#define _GNU_SOURCE
#include <stdio.h>

typedef struct {
    int number;
    double cash_payments;
} transaction_t;

int transaction_read(FILE *in, transaction_t *transaction);
int transaction_print(FILE *out, const transaction_t *transaction);
int transactions_input_output_from_in_to_out(FILE *in, FILE *out);

#endif  // PROJECT_INCLUDE_TRANSACTIONS_H_
