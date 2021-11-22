#include <stdlib.h>
#include "utils.h"
#include "is_primer.h"
#include "print_numbers.h"

#define ERR_ARGS_COUNT (-1)
#define ERR_WRONG_FLG (-2)

int main(int argc, const char** argv) {
    enum action {COUNT_TICKS = 1, EXPONENTIATION, PRIME_CHECK, PRINT_RANGE};

    if (argc < 3) {
        return ERR_ARGS_COUNT;
    }

    int test_case = atoi(argv[1]);
    const char* data = argv[2];

    switch (test_case) {
        case COUNT_TICKS: {
            int to = atoi(data);
            size_t ticks_count = timer_from(to);
            printf("%zu\n", ticks_count);
            break;
        }
        case EXPONENTIATION: {
            if (argc != 4)
                return ERR_ARGS_COUNT;

            int base = atoi(data);
            int pow =  atoi(argv[3]);
            int res = (int)custom_pow(base, pow);

            printf("%d\n", res);

            break;
        }
        case PRIME_CHECK: {
            int num = atoi(data);
            printf("%d", is_prime_num(num));
            break;
        }
        case PRINT_RANGE: {
            int num = atoi(data);
            print_from_one_to_n(num);
            break;
        }
        default: {
            return ERR_WRONG_FLG;
        }
    }
}
