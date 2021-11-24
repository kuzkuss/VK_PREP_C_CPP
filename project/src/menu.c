#include <stdio.h>

#include "clients.h"
#include "errors.h"
#include "menu.h"
#include "transactions.h"
#include "update_base.h"

static const char *filename_transactions = "transactions.dat";
static const char *filename_clients = "clients.dat";
static const char *filename_new_base = "new_base.dat";

int client_base_processing(void) {
    FILE *file_clients = NULL;
    FILE *file_transactions = NULL;
    FILE *file_new_base = NULL;
    int choice = 0;
    int rc = OK;
    puts("please enter action\n1 enter data client:\n2 enter data transaction:\n3 update base");
    while (scanf("%d", &choice) == 1) {
        switch (choice) {
            case 1: {
                file_clients = fopen(filename_clients, "r+");
                if (!file_clients) {
                    return FILE_ERROR;
                }
                rc = clients_input_output_from_in_to_out(stdin, file_clients);
                fclose(file_clients);
                if (rc) {
                    return rc;
                }
                break;
            }
            case 2: {
                file_transactions = fopen(filename_transactions, "r+");
                if (!file_transactions) {
                    return FILE_ERROR;
                }
                rc = transactions_input_output_from_in_to_out(stdin, file_transactions);
                fclose(file_transactions);
                if (rc) {
                    return rc;
                }
                break;
            }
            case 3: {
                file_clients = fopen(filename_clients, "r");
                file_transactions = fopen(filename_transactions, "r");
                file_new_base = fopen(filename_new_base, "w");

                if (!file_clients) {
                    return FILE_ERROR;
                }

                if (!file_transactions) {
                    fclose(file_clients);
                    puts("File error\n");
                    return FILE_ERROR;
                }

                if (!file_new_base) {
                    fclose(file_clients);
                    fclose(file_transactions);
                    return FILE_ERROR;
                }
                rc = update_info(file_clients, file_transactions, file_new_base);
                fclose(file_clients);
                fclose(file_transactions);
                fclose(file_new_base);
                if (rc) {
                    return rc;
                }
                break;
            }
            default: {
                puts("Incorrect choice\n");
                break;
            }
        }
        puts("please enter action\n1 enter data client:\n2 enter data transaction:\n3 update base");
    }
    return rc;
}
