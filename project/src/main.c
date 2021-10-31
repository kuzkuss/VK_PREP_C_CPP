#include <stdio.h>
#include "matrix.h"

int main(void) {
    int rows = 4;
    int columns = 4;
    Matrix *matrix = create_matrix(rows, columns);
//    Matrix *mul_mtr = mul_scalar(matrix, 4.2);
//    Matrix *transp_mtr = transp(matrix);
//    free_matrix(mul_mtr);
//    free_matrix(transp_mtr);
    free_matrix(matrix);
    return 0;
}

