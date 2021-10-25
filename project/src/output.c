#include <stdio.h>
#include "output.h"

void print_info_client(void) {
    puts("1 Account number: \n"
         "2 Client name: \n"
         "3 Surname: \n"
         "4 Client address: \n"
         "5 Client telephone number: \n"
         "6 Client indebtedness: \n"
         "7 Client credit limit: \n"
         "8 Client cash payments: \n");
}

void print_info_transaction(void) {
    puts("1 Number account: \n"
         "2 Client cash payments: ");
}

void print_menu(void) {
    puts("please enter action\n1 enter data client:\n2 enter data transaction:\n3 update base");
}

void print_to_file(FILE *f, const Data *client_data) {
    fprintf(f, "%-12d%-11s%-11s%-16s%20s%12.2f%12.2f%12.2f\n", client_data->number,
            client_data->name, client_data->surname, client_data->address, client_data->tel_number,
            client_data->indebtedness, client_data->credit_limit, client_data->cash_payments);
}

