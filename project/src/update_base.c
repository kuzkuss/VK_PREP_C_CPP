#include "update_base.h"
#include "output.h"

void update_info(FILE *f_clients, FILE *f_transactions, FILE *blackrecord, Data client_data, Data transfer) {
    while (fscanf(f_clients, "%d%19s%19s%29s%14s%lf%lf%lf", &client_data.number, client_data.name,
                  client_data.surname, client_data.address, client_data.tel_number, &client_data.indebtedness,
                  &client_data.credit_limit, &client_data.cash_payments) == 8) {
        while (fscanf(f_transactions, "%d %lf", &transfer.number, &transfer.cash_payments) == 2) {
            if (client_data.number == transfer.number && transfer.cash_payments != 0)
                client_data.credit_limit += transfer.cash_payments;
        }
        print_to_file(blackrecord, client_data);
        rewind(f_transactions);
    }
}

