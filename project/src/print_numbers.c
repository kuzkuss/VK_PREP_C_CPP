#include <stdio.h>
#include <stdlib.h>

#include "print_numbers.h"

#define FROM 1

static void print_range(int from, int to);

void print_from_one_to_n(int n) {
    print_range(1, n);
}

static void print_range(int from, int to) {
    if (to == FROM) {
        printf("%d", to);
        return;
    }
    if (to < FROM)
        print_range(from, to + 1);
    else
        print_range(from, to - 1);

    printf(" %d", to);
}

