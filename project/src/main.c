#include <stdio.h>
#include "matrix.h"

int main(void) {
    int rows = 4;
    int columns = 4;
    Matrix *matrix = create_matrix(rows, columns);
    free_matrix(matrix);
    return 0;
}
