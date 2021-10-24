#include <stdio.h>
#include "utils.h"
#include "data_client.h"
#include "data_transaction.h"
#include "update_base.h"
#include "output.h"

int main(void) {
    setbuf(stdout, NULL);
    int choice = 0;
    FILE *file_record = NULL;
    FILE *file_transaction = NULL;
    FILE *blackrecord = NULL;
    Data client_data = {0};
    Data transfer = {0};
    print_menu();
    while (scanf("%d", &choice) == 1) {
        switch (choice) {
            case 1:
                file_record = fopen(filename_record, "r+");
                if (file_record == NULL) {
                    puts("Not access");
                } else {
                    write_clients(file_record, client_data);
                    fclose(file_record);
                }
                break;
            case 2:
                file_record = fopen(filename_transaction, "r+");
                if (file_record == NULL) {
                    puts("Not access");
                } else {
                    write_transactions(file_record, transfer);
                    fclose(file_record);
                }
                break;
            case 3:
                file_record = fopen(filename_record, "r");
                file_transaction = fopen(filename_transaction, "r");
                blackrecord = fopen(filename_blackrecord, "w");
                if (file_record == NULL || file_transaction == NULL || blackrecord == NULL) {
                    puts("exit");
                } else {
                    update_info(file_record, file_transaction, blackrecord, client_data, transfer);
                    fclose(file_record);
                    fclose(file_transaction);
                    fclose(blackrecord);
                }
                break;
            default:
                puts("error");
                break;
        }
        print_menu();
    }
    return 0;
}

