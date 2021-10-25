#include <stdio.h>
#include "utils.h"
#include "data_client.h"
#include "data_transaction.h"
#include "update_base.h"
#include "output.h"

int main(void) {
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
                file_record = fopen(FILENAME_RECORD, "r+");
                if (file_record == NULL) {
                    puts("No access");
                } else {
                    write_clients(file_record, &client_data);
                    fclose(file_record);
                }
                break;
            case 2:
                file_record = fopen(FILENAME_TRANSACTION, "r+");
                if (file_record == NULL) {
                    puts("No access");
                } else {
                    write_transactions(file_record, &transfer);
                    fclose(file_record);
                }
                break;
            case 3:
                file_record = fopen(FILENAME_RECORD, "r");
                file_transaction = fopen(FILENAME_TRANSACTION, "r");
                blackrecord = fopen(FILENAME_BLACKRECORD, "w");
                if (file_record == NULL) {
                    puts("exit");
                } else if (file_transaction == NULL) {
                    fclose(file_record);
                    puts("exit");
                } else if (blackrecord == NULL) {
                    fclose(file_record);
                    fclose(file_transaction);
                    puts("exit");
                } else {
                    update_info(file_record, file_transaction, blackrecord, &client_data, &transfer);
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

