#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clients.h"
#include "errors.h"
#include "transactions.h"
#include "update_base.h"

#define EPS 1e-7

static const char *filename_transactions = "transactions.dat";
static const char *filename_clients = "clients.dat";
static const char *filename_new_base = "new_base.dat";

static void equality_check(const client_t *expected_data, const client_t *got_data);
static void test_client_read_print(void);
static void test_transaction_read_print(void);
static void test_update_base(void);

int main(void) {
    test_client_read_print();

    test_transaction_read_print();

    test_update_base();

    puts("SUCCESS");

    return OK;
}

void test_client_read_print(void) {
    client_t expected_data = {
        .number = 1,
        .name = "name_1",
        .surname = "surname_1",
        .address = "address_1",
        .telephone = "89201909927",
        .indebtedness = 50,
        .credit_limit = 100,
        .cash_payments = 20
    };

    FILE *file_clients = fopen(filename_clients, "w+");
    assert(file_clients != NULL);

    int rc = client_print(file_clients, &expected_data);
    assert(rc == OK);

    rewind(file_clients);

    client_t got_data = { 0 };
    rc = client_read(file_clients, &got_data);
    assert(rc == OK);

    equality_check(&expected_data, &got_data);

    free(got_data.name);
    free(got_data.surname);
    free(got_data.address);
    free(got_data.telephone);
    fclose(file_clients);
}

void test_transaction_read_print(void) {
    transaction_t expected_data = {
        .number = 1,
        .cash_payments = 10
    };

    FILE *file_transactions = fopen(filename_transactions, "w+");
    assert(file_transactions != NULL);

    int rc = transaction_print(file_transactions, &expected_data);
    assert(rc == OK);

    rewind(file_transactions);

    transaction_t got_data = { 0 };
    rc = transaction_read(file_transactions, &got_data);
    assert(rc == OK);

    assert(expected_data.number == got_data.number);
    assert(fabs(expected_data.cash_payments - got_data.cash_payments) < EPS);

    fclose(file_transactions);
}

void test_update_base(void) {
    client_t client = {
        .number = 1,
        .name = "name_1",
        .surname = "surname_1",
        .address = "address_1",
        .telephone = "89201909927",
        .indebtedness = 50,
        .credit_limit = 100,
        .cash_payments = 20
    };

    transaction_t transaction = {
        .number = 1,
        .cash_payments = 10
    };

    FILE *file_clients = fopen(filename_clients, "r+");
    assert(file_clients != NULL);

    int rc = client_print(file_clients, &client);
    assert(rc == OK);

    FILE *file_transactions = fopen(filename_transactions, "r+");
    assert(file_transactions != NULL);

    rc = transaction_print(file_transactions, &transaction);
    assert(rc == OK);

    FILE *file_new_base = fopen(filename_new_base, "w+");
    assert(file_new_base != NULL);

    rewind(file_clients);
    rewind(file_transactions);

    rc = update_info(file_clients, file_transactions, file_new_base);
    assert(rc == OK);

    rewind(file_new_base);

    client_t got_data = { 0 };
    rc = client_read(file_new_base, &got_data);
    assert(rc == OK);

    client.credit_limit += transaction.cash_payments;

    equality_check(&client, &got_data);

    free(got_data.name);
    free(got_data.surname);
    free(got_data.address);
    free(got_data.telephone);

    fclose(file_clients);
    fclose(file_transactions);
    fclose(file_new_base);
}

static void equality_check(const client_t *expected_data, const client_t *got_data) {
    assert(expected_data->number == got_data->number);
    assert(strcmp(expected_data->name, got_data->name) == 0);
    assert(strcmp(expected_data->surname, got_data->surname) == 0);
    assert(strcmp(expected_data->address, got_data->address) == 0);
    assert(strcmp(expected_data->telephone, got_data->telephone) == 0);
    assert(fabs(expected_data->indebtedness - got_data->indebtedness) < EPS);
    assert(fabs(expected_data->credit_limit - got_data->credit_limit) < EPS);
    assert(fabs(expected_data->cash_payments - got_data->cash_payments) < EPS);
}

