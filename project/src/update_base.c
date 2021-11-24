#include <stdlib.h>

#include "clients.h"
#include "errors.h"
#include "update_base.h"
#include "transactions.h"

int update_info(FILE *file_in_clients, FILE *file_in_transactions, FILE *file_out_new_base) {
    if (!file_in_clients || !file_in_transactions || !file_out_new_base)
        return INPUT_OUTPUT_ERROR;
    client_t client = { 0 };
    transaction_t transaction = { 0 };

    int rc = INPUT_OUTPUT_ERROR;

    while (client_read(file_in_clients, &client) == OK) {
        while (transaction_read(file_in_transactions, &transaction) == OK) {
            if (client.number == transaction.number && transaction.cash_payments != 0)
                client.credit_limit += transaction.cash_payments;
        }
        client_print(file_out_new_base, &client);
        rewind(file_in_transactions);
        free(client.name);
        free(client.surname);
        free(client.address);
        free(client.telephone);
    }

    if (feof(file_in_clients))
        rc = OK;

    return rc;
}

