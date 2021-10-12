#include <stdio.h>
#include <stdlib.h>
#include "print_numbers.h"

void print_all_numbers(int n)
{
    if (n == 1)
        printf("%d", n);
    else
    {
        if (n <= 0)
            print_all_numbers(n + 1);
        else
            print_all_numbers(n - 1);
        printf(" %d", n);
    }
}
