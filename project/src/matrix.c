#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

#define EPS 1.0e-7

static void fill_new_matrix(const Matrix* matrix, Matrix* new_matrix, size_t del_col, size_t del_row);
static Matrix* arithmetic_operation(const Matrix* l, const Matrix* r, int action);

enum ERROR_CODE {ERROR_CODE_OK, ERROR_CODE_INCORRECT_DATA, ERROR_CODE_MEMORY_ALLOCATION_ERROR};

enum ACTION {ACTION_SUM = 1, ACTION_SUB = -1};

Matrix* create_matrix_from_file(const char* path_file) {
    if (!path_file) {
        return NULL;
    }

    FILE *f_in = fopen(path_file, "r");
    if (!f_in) {
        return NULL;
    }

    int rows = 0;
    int columns = 0;
    if (fscanf(f_in, "%d", &rows) != 1 || rows <= 0) {
        fclose(f_in);
        return NULL;
    }
    if (fscanf(f_in, "%d", &columns) != 1 || columns <= 0) {
        fclose(f_in);
        return NULL;
    }

    Matrix *mtr = create_matrix(rows, columns);
    if (!mtr) {
        fclose(f_in);
        return NULL;
    }

    int rc = ERROR_CODE_OK;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (fscanf(f_in, "%lf", &mtr->elements[i * columns + j]) != 1) {
                free_matrix(mtr);
                rc = ERROR_CODE_INCORRECT_DATA;
                break;
            }
        }
        if (rc) {
            break;
        }
    }

    fclose(f_in);

    if (rc) {
        return NULL;
    }

    return mtr;
}

Matrix* create_matrix(size_t rows, size_t cols) {
    if (rows == 0 || cols == 0) {
        return NULL;
    }

    Matrix *mtr = calloc(1, sizeof(Matrix));
    if (!mtr) {
        return NULL;
    }

    mtr->elements = calloc(rows * cols, sizeof(double));
    if (!mtr->elements) {
        free(mtr);
        return NULL;
    }

    mtr->rows = rows;
    mtr->columns = cols;

    return mtr;
}

void free_matrix(Matrix* matrix) {
    if (matrix) {
        free(matrix->elements);
        matrix->elements = NULL;
        free(matrix);
    }
}

int get_rows(const Matrix* matrix, size_t* rows) {
    if (!matrix || !matrix->elements || !rows) {
        return ERROR_CODE_INCORRECT_DATA;
    }
    *rows = matrix->rows;
    return ERROR_CODE_OK;
}

int get_cols(const Matrix* matrix, size_t* cols) {
    if (!matrix || !matrix->elements || !cols) {
        return ERROR_CODE_INCORRECT_DATA;
    }
    *cols = matrix->columns;
    return ERROR_CODE_OK;
}

int get_elem(const Matrix* matrix, size_t row, size_t col, double* val) {
    if (!matrix || !matrix->elements || row >= matrix->rows || col >= matrix->columns || !val) {
        return ERROR_CODE_INCORRECT_DATA;
    }
    *val = matrix->elements[row * matrix->columns + col];
    return ERROR_CODE_OK;
}

int set_elem(Matrix* matrix, size_t row, size_t col, double val) {
    if (!matrix || !matrix->elements || row >= matrix->rows || col >= matrix->columns) {
        return ERROR_CODE_INCORRECT_DATA;
    }
    matrix->elements[row * matrix->columns + col] = val;
    return ERROR_CODE_OK;
}

Matrix* mul_scalar(const Matrix* matrix, double val) {
    if (!matrix || !matrix->elements) {
        return NULL;
    }
    Matrix *new_mtr = create_matrix(matrix->rows, matrix->columns);
    if (!new_mtr) {
        return NULL;
    }
    for (size_t i = 0; i < matrix->rows; ++i) {
        for (size_t j = 0; j < matrix->columns; ++j) {
            new_mtr->elements[i * matrix->columns + j] = val * matrix->elements[i * matrix->columns + j];
        }
    }
    return new_mtr;
}

Matrix* transp(const Matrix* matrix) {
    if (!matrix || !matrix->elements) {
        return NULL;
    }
    Matrix *transp_mtr = create_matrix(matrix->columns, matrix->rows);
    if (!transp_mtr) {
        return NULL;
    }
    for (size_t i = 0; i < matrix->columns; ++i) {
        for (size_t j = 0; j < matrix->rows; ++j) {
            transp_mtr->elements[i * matrix->rows + j] = matrix->elements[j * matrix->columns + i];
        }
    }
    return transp_mtr;
}

static Matrix* arithmetic_operation(const Matrix* l, const Matrix* r, int action) {
    if (!l || !l->elements || !r || !r->elements) {
        return NULL;
    }
    if (l->rows != r->rows || l->columns != r->columns) {
        return NULL;
    }

    Matrix *res_mtr = create_matrix(l->rows, l->columns);
    if (!res_mtr) {
        return NULL;
    }

    for (size_t i = 0; i < l->rows; ++i) {
        for (size_t j = 0; j < l->columns; ++j) {
            res_mtr->elements[i * l->columns + j] =
                    l->elements[i * l->columns + j] + action * r->elements[i * r->columns + j];
        }
    }
    return res_mtr;
}

Matrix* sum(const Matrix* l, const Matrix* r) {
    Matrix *res_mtr = arithmetic_operation(l, r, ACTION_SUM);
    return res_mtr;
}

Matrix* sub(const Matrix* l, const Matrix* r) {
    Matrix *res_mtr = arithmetic_operation(l, r, ACTION_SUB);
    return res_mtr;
}

Matrix* mul(const Matrix* l, const Matrix* r) {
    if (!l || !l->elements || !r || !r->elements) {
        return NULL;
    }
    if (l->columns != r->rows) {
        return NULL;
    }

    Matrix *res_mtr = create_matrix(l->rows, r->columns);
    if (!res_mtr) {
        return NULL;
    }

    for (size_t i = 0; i < l->rows; ++i) {
        for (size_t j = 0; j < l->columns; ++j) {
            for (size_t k = 0; k < r->columns; ++k) {
                res_mtr->elements[i * r->columns + k] +=
                        l->elements[i * l->columns + j] * r->elements[j * r->columns + k];
            }
        }
    }
    return res_mtr;
}

static void fill_new_matrix(const Matrix* matrix, Matrix* new_matrix, size_t del_col, size_t del_row) {
    if (!matrix || !matrix->elements || !new_matrix ||
            !new_matrix->elements || del_col >= matrix->columns || del_row >= matrix->rows) {
        return;
    }

    size_t new_i = 0;
    size_t new_j = 0;
    for (size_t i = 0; i < matrix->rows; ++i) {
        for (size_t j = 0; j < matrix->rows; ++j) {
            if (j == del_col || i == del_row) {
                continue;
            }
            new_matrix->elements[new_i * new_matrix->columns + new_j] =
                    matrix->elements[i * matrix->columns + j];
            if (++new_j == matrix->rows - 1) {
                new_j = 0;
                ++new_i;
            }
        }
    }
}

int det(const Matrix* matrix, double* val) {
    if (!matrix || !matrix->elements || !val) {
        return ERROR_CODE_INCORRECT_DATA;
    }

    if (matrix->rows != matrix->columns) {
        return ERROR_CODE_INCORRECT_DATA;
    }

    if (matrix->rows == 1) {
        *val = matrix->elements[0];
        return ERROR_CODE_OK;
    }

    Matrix *new_mtr = create_matrix(matrix->rows - 1, matrix->columns - 1);
    if (!new_mtr) {
        return ERROR_CODE_MEMORY_ALLOCATION_ERROR;
    }

    int rc = ERROR_CODE_OK;

    *val = 0.0;
    for (size_t i = 0; i < matrix->rows; ++i) {
        fill_new_matrix(matrix, new_mtr, i, 0);
        double cur_res = 0.0;
        rc = det(new_mtr, &cur_res);
        if (rc) {
            break;
        }
        if (i % 2 != 0)
            cur_res = -cur_res;
        *val += matrix->elements[i] * cur_res;
    }
    free_matrix(new_mtr);
    return rc;
}

Matrix* adj(const Matrix* matrix) {
    if (!matrix || !matrix->elements) {
        return NULL;
    }
    if (matrix->rows != matrix->columns) {
        return NULL;
    }
    if (matrix->rows == 1) {
        return NULL;
    }

    Matrix *adj_mtr = transp(matrix);
    if (!adj_mtr) {
        return NULL;
    }

    double res = 0.0;
    Matrix *new_mtr = create_matrix(matrix->rows - 1, matrix->columns - 1);
    if (!new_mtr) {
        return NULL;
    }

    int rc = ERROR_CODE_OK;
    for (size_t i = 0; i < matrix->rows; ++i) {
        for (size_t j = 0; j < matrix->columns; ++j) {
            fill_new_matrix(matrix, new_mtr, i, j);
            rc = det(new_mtr, &res);
            if (rc) {
                free_matrix(adj_mtr);
                i = matrix->columns;
                break;
            }
            if ((i + j) % 2 != 0) {
                res = -res;
            }

            adj_mtr->elements[i * matrix->columns + j] = res;
        }
    }
    free_matrix(new_mtr);

    if (rc) {
        return NULL;
    }

    return adj_mtr;
}

Matrix* inv(const Matrix* matrix) {
    if (!matrix || !matrix->elements) {
        return NULL;
    }
    if (matrix->rows != matrix->columns) {
        return NULL;
    }
    double det_mtr = 0.0;
    int rc = det(matrix, &det_mtr);
    if (rc) {
        return NULL;
    }

    if (fabs(det_mtr - 0.0) < EPS) {
        return NULL;
    }

    if (matrix->rows == 1) {
        Matrix *adj_mtr = create_matrix(1, 1);
        if (!adj_mtr) {
            return NULL;
        }
        adj_mtr->elements[0] = 1 / det_mtr;
        return adj_mtr;
    }

    Matrix *adj_mtr = adj(matrix);
    if (!adj_mtr) {
        return NULL;
    }

    Matrix *inv_mtr = mul_scalar(adj_mtr, 1 / det_mtr);
    if (!inv_mtr) {
        free_matrix(adj_mtr);
        return NULL;
    }
    free_matrix(adj_mtr);
    return inv_mtr;
}

