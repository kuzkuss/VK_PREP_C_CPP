#include "errors.h"
#include "transactions.h"

int transaction_read(FILE *in, transaction_t *transaction) {
    if (!in || !transaction)
        return INCORRECT_ARGS;

    int rc = INPUT_OUTPUT_ERROR;

    if (fscanf(in, "%d", &transaction->number) == 1 && transaction->number > 0) {
        if (fscanf(in, "%lf\n", &transaction->cash_payments) == 1 || transaction->cash_payments >= 0) {
            rc = OK;
        }
    }

    printf("%d", rc);
    return rc;
}

int transaction_print(FILE *out, const transaction_t *transaction) {
    if (!out || !transaction)
        return INPUT_OUTPUT_ERROR;

    fprintf(out, "%-12d", transaction->number);
    fprintf(out, "%12.2f\n", transaction->cash_payments);
    return OK;
}

int transactions_input_output_from_in_to_out(FILE *in, FILE *out) {
    if (!in || !out)
        return INCORRECT_ARGS;

    int rc = OK;
    transaction_t transaction = { 0 };
    do {
        puts("Enter information about transaction:\n"
             "1 Account number\n"
             "2 Cash payments\n");
        rc = transaction_read(in, &transaction);
        if (!rc) {
            rc = transaction_print(out, &transaction);
        }
    } while (rc == OK);

    if (!feof(in))
        rc = INPUT_OUTPUT_ERROR;

    return rc;
}
