#include <stdio.h>
#include <string.h>
#include <math.h>
#include "update_base.h"
#include "output.h"

#define EPS 1e-7

#define ERROR 1
#define OK 0

int test_program(void);

int main(void) {
    if (test_program() == OK)
        puts("Test succeed.");
    else
        puts("Test failed.");
    return OK;
}

int test_program(void) {
    int rc = 0;

    const char *filename_clients = "record.dat";
    FILE *file_record = fopen(filename_clients, "r+");
    if (!file_record) {
        puts("No access.");
        return ERROR;
    }
    Data client = {1, "name_1", "surname_1", "address_1", "89201909927", 50, 100, 20};

    print_to_file(file_record, &client);

    const char *filename_transactions = "transaction.dat";
    FILE *file_transaction = fopen(filename_transactions, "r+");
    if (!file_transaction) {
        puts("No access.");
        fclose(file_record);
        return ERROR;
    }
    Data transfer = {0};
    transfer.number = 1;
    transfer.cash_payments = 10;
    fprintf(file_transaction, "%-3d%-6.2lf\n", transfer.number, transfer.cash_payments);

    const char *filename_res = "blackrecord.dat";
    FILE *blackrecord = fopen(filename_res, "w+");
    if (!blackrecord) {
        puts("Not access.");
        fclose(file_record);
        fclose(file_transaction);
        return ERROR;
    }
    rewind(file_record);
    rewind(file_transaction);
    update_info(file_record, file_transaction, blackrecord, &client, &transfer);

    client.credit_limit += transfer.cash_payments;

    Data got_data = {0};

    rewind(blackrecord);

    if (fscanf(blackrecord, "%d", &got_data.number) != 1 || got_data.number != client.number)
        rc = ERROR;

    if (fscanf(blackrecord, "%19s", got_data.name) != 1 || strcmp(got_data.name, client.name) != 0)
        rc = ERROR;

    if (fscanf(blackrecord, "%19s", got_data.surname) != 1 ||
                strcmp(got_data.surname, client.surname) != 0)
        rc = ERROR;

    if (fscanf(blackrecord, "%29s", got_data.address) != 1 ||
                strcmp(got_data.address, client.address) != 0)
        rc = ERROR;

    if (fscanf(blackrecord, "%14s", got_data.tel_number) != 1 ||
                strcmp(got_data.tel_number, client.tel_number))
        rc = ERROR;

    if (fscanf(blackrecord, "%lf", &got_data.indebtedness) != 1 ||
        fabs(got_data.indebtedness - client.indebtedness) >= EPS)
        rc = ERROR;

    if (fscanf(blackrecord, "%lf", &got_data.credit_limit) != 1 ||
        fabs(got_data.credit_limit - client.credit_limit) >= EPS)
        rc = ERROR;

    if (fscanf(blackrecord, "%lf", &got_data.cash_payments) != 1 ||
        fabs(got_data.cash_payments - client.cash_payments) >= EPS)
        rc = ERROR;

    fclose(file_record);
    fclose(file_transaction);
    fclose(blackrecord);
    return rc;
}

