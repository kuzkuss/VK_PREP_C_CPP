#include "data_transaction.h"
#include "output.h"

void write_transactions(FILE *f, Data transfer) {
    print_info_transaction();
    while (scanf("%d %lf", &transfer.Number, &transfer.cash_payments) == 2) {
        fprintf(f, "%-3d%-6.2lf\n", transfer.Number, transfer.cash_payments);
        print_info_transaction();
    }
}

