#include "data_client.h"
#include "output.h"

void write_clients(FILE *f, Data *client_data) {
    print_info_client();
    while (scanf(FORMAT_STRING, &client_data->number, client_data->name, client_data->surname,
                 client_data->address, client_data->tel_number, &client_data->indebtedness,
                 &client_data->credit_limit, &client_data->cash_payments) == 8) {
        print_to_file(f, client_data);
        print_info_client();
    }
}

