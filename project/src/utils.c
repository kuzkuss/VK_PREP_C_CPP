#include "utils.h"

size_t timer_from(int from) {
    size_t counter = 0;
    for (int i = from; i >= 0; --i) {
        ++counter;
        i ? printf("%d ", i) : printf("%d\n", i);
    }
    return counter;
}

// TODO: Implement `power of` function

int custom_pow(int base, int power) {
    if (power == 0)
        return 1;
    int result = base;
    for (int i = 1; i < power; ++i)
        result *= base;
    return result;
}

