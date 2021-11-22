#include "is_primer.h"

#define FALSE 0
#define TRUE 1

int is_prime_num(int num) {
    int result = TRUE;

    if (num < 2) {
        result = FALSE;
        return result;
    }

    for (int delimiter = 2; delimiter * delimiter <= num; ++delimiter) {
        if (num % delimiter == 0) {
            result = FALSE;
            break;
        }
    }

    return result;
}

