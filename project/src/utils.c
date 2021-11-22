#include <stdlib.h>

#include "utils.h"

size_t timer_from(int from) {
    size_t counter = 0;

    if (from <= 0)
        return counter;

    for (int i = from; i > 0; --i) {
        ++counter;
        printf("%d ", i);
    }

    puts("0");

    return counter;
}

double custom_pow(int base, int power) {
    if (power == 0)
        return 1;

    double result = base;

    for (int i = 1; i < abs(power); ++i)
        result *= base;

    if (power < 0)
        result = 1 / result;

    return result;
}

