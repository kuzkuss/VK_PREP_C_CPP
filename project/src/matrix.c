#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"

Matrix* create_matrix_from_file(const char* path_file) {
    FILE *f_in = fopen(path_file, "r");
    if (!f_in)
        return NULL;
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

    Matrix *mtr = calloc(1, sizeof(Matrix));
    if (!mtr) {
        fclose(f_in);
        return NULL;
    }

    mtr->prows = malloc(rows * sizeof(double *));
    if (!mtr->prows) {
        free(mtr);
        fclose(f_in);
        return NULL;
    }

    mtr->elements = calloc(rows * columns, sizeof(double));
    if (!mtr->elements) {
        free(mtr->prows);
        free(mtr);
        fclose(f_in);
        return NULL;
    }

    for (int i = 0; i < rows; ++i)
        mtr->prows[i] = mtr->elements + i * columns;

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            if (fscanf(f_in, "%lf", &mtr->prows[i][j]) != 1) {
                free_matrix(mtr);
                fclose(f_in);
                return NULL;
            }

    mtr->rows = rows;
    mtr->columns = columns;

    fclose(f_in);
    return mtr;
}

Matrix* create_matrix(size_t rows, size_t cols) {
    Matrix *mtr = calloc(1, sizeof(Matrix));
    if (!mtr)
        return NULL;

    mtr->prows = malloc(rows * sizeof(double *));
    if (!mtr->prows) {
        free(mtr);
        return NULL;
    }

    mtr->elements = calloc(rows * cols, sizeof(double));
    if (!mtr->elements) {
        free(mtr->prows);
        free(mtr);
        return NULL;
    }

    for (size_t i = 0; i < rows; ++i)
        mtr->prows[i] = mtr->elements + i * cols;

    mtr->rows = rows;
    mtr->columns = cols;

    return mtr;
}

void free_matrix(Matrix* matrix) {
    free(matrix->elements);
    matrix->elements = NULL;
    free(matrix->prows);
    matrix->prows = NULL;
    free(matrix);
}

int get_rows(const Matrix* matrix, size_t* rows) {
    if (!matrix || !matrix->prows)
        return ERROR;
    *rows = matrix->rows;
    return OK;
}

int get_cols(const Matrix* matrix, size_t* cols) {
    if (!matrix || !matrix->elements)
        return ERROR;
    *cols = matrix->columns;
    return OK;
}

int get_elem(const Matrix* matrix, size_t row, size_t col, double* val) {
    if (!matrix || !matrix->prows)
        return ERROR;
    *val = matrix->prows[row][col];
    return OK;
}

int set_elem(Matrix* matrix, size_t row, size_t col, double val) {
    if (!matrix || !matrix->prows)
        return ERROR;
    matrix->prows[row][col] = val;
    return OK;
}

Matrix* mul_scalar(const Matrix* matrix, double val) {
    if (!matrix || !matrix->prows || !matrix->elements)
        return NULL;
    Matrix *new_mtr = create_matrix(matrix->rows, matrix->columns);
    if (!new_mtr)
        return NULL;
    for (size_t i = 0; i < matrix->rows; ++i)
        for (size_t j = 0; j < matrix->columns; ++j)
            new_mtr->prows[i][j] = val * matrix->prows[i][j];
    return new_mtr;
}

Matrix* transp(const Matrix* matrix) {
    if (!matrix || !matrix->prows || !matrix->elements)
        return NULL;
    Matrix *transp_mtr = create_matrix(matrix->columns, matrix->rows);
    if (!transp_mtr)
        return NULL;
    for (size_t i = 0; i < matrix->columns; ++i)
        for (size_t j = 0; j < matrix->rows; ++j)
            transp_mtr->prows[i][j] = matrix->prows[j][i];
    return transp_mtr;
}

Matrix* sum(const Matrix* l, const Matrix* r) {
    if (!l || !l->prows || !l->elements || !r || !r->prows || !r->elements)
        return NULL;
    if (l->rows != r->rows || l->columns != r->columns)
        return NULL;
    Matrix *res_mtr = create_matrix(l->rows, l->columns);
    if (!res_mtr)
        return NULL;
    for (size_t i = 0; i < l->rows; ++i)
        for (size_t j = 0; j < l->columns; ++j)
            res_mtr->prows[i][j] = l->prows[i][j] + r->prows[i][j];
    return res_mtr;
}

Matrix* sub(const Matrix* l, const Matrix* r) {
    if (!l || !l->prows || !l->elements || !r || !r->prows || !r->elements)
        return NULL;
    if (l->rows != r->rows || l->columns != r->columns)
        return NULL;
    Matrix *res_mtr = create_matrix(l->rows, l->columns);
    if (!res_mtr)
        return NULL;
    for (size_t i = 0; i < l->rows; ++i)
        for (size_t j = 0; j < l->columns; ++j)
            res_mtr->prows[i][j] = l->prows[i][j] - r->prows[i][j];
    return res_mtr;
}

Matrix* mul(const Matrix* l, const Matrix* r) {
    if (!l || !l->prows || !l->elements || !r || !r->prows || !r->elements)
        return NULL;
    if (l->columns != r->rows)
        return NULL;
    Matrix *res_mtr = create_matrix(l->rows, r->columns);
    if (!res_mtr)
        return NULL;
    for (size_t i = 0; i < l->rows; ++i)
        for (size_t j = 0; j < l->columns; ++j)
            for (size_t k = 0; k < r->columns; ++k)
                res_mtr->prows[i][k] += l->prows[i][j] * r->prows[j][k];
    return res_mtr;
}

void fill_new_matrix(const Matrix* matrix, Matrix* new_matrix, int n, int del_col, int del_row) {
    int new_i = 0;
    int new_j = 0;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            if (j != del_col && i != del_row) {
                new_matrix->prows[new_i][new_j] = matrix->prows[i][j];
                if (++new_j == n - 1) {
                    new_j = 0;
                    ++new_i;
                }
            }
        }
}

int det(const Matrix* matrix, double* val) {
    if (!matrix || !matrix->prows || !matrix->elements)
        return ERROR;

    if (matrix->rows != matrix->columns)
        return ERROR;

    if (matrix->rows == 1) {
        *val = matrix->prows[0][0];
        return OK;
    }


    Matrix *new_mtr = create_matrix(matrix->rows - 1, matrix->columns - 1);
    if (!new_mtr)
        return ERROR;
    *val = 0.0;
    for (size_t i = 0; i < matrix->rows; ++i) {
        fill_new_matrix(matrix, new_mtr, matrix->rows, i, 0);
        double cur_res = 0.0;
        int rc = det(new_mtr, &cur_res);
        if (rc) {
            free_matrix(new_mtr);
            return ERROR;
        }
        *val += matrix->prows[0][i] * pow((-1), i + 2) * cur_res;
    }
    free_matrix(new_mtr);
    return OK;
}

Matrix* adj(const Matrix* matrix) {
    if (!matrix || !matrix->prows || !matrix->elements)
        return NULL;
    if (matrix->rows != matrix->columns)
        return NULL;
    if (matrix->rows == 1)
        return NULL;

    Matrix *adj_mtr = transp(matrix);
    if (!adj_mtr)
        return NULL;

    double res = 0.0;
    Matrix *new_mtr = create_matrix(matrix->rows - 1, matrix->columns - 1);
    if (!new_mtr)
        return NULL;
    for (size_t i = 0; i < matrix->columns; ++i)
        for (size_t j = 0; j < matrix->rows; ++j) {
            fill_new_matrix(matrix, new_mtr, matrix->rows, i, j);
            int rc = det(new_mtr, &res);
            if (rc) {
                free_matrix(adj_mtr);
                free_matrix(new_mtr);
                return NULL;
            }
            adj_mtr->prows[i][j] = pow((-1), i + j + 2) * res;
        }
    free_matrix(new_mtr);
    return adj_mtr;
}

Matrix* inv(const Matrix* matrix) {
    if (!matrix || !matrix->prows || !matrix->elements)
        return NULL;
    if (matrix->rows != matrix->columns)
        return NULL;
    double det_mtr = 0.0;
    int rc = det(matrix, &det_mtr);
    if (rc)
        return NULL;

    if (det_mtr == 0)
        return NULL;

    if (matrix->rows == 1) {
        Matrix *adj_mtr = create_matrix(1, 1);
        adj_mtr->prows[0][0] = 1 / det_mtr;
        return adj_mtr;
    }

    Matrix *adj_mtr = adj(matrix);
    if (!adj_mtr)
        return NULL;

    Matrix *inv_mtr = mul_scalar(adj_mtr, 1 / det_mtr);
    if (!inv_mtr) {
        free_matrix(adj_mtr);
        return NULL;
    }
    free_matrix(adj_mtr);
    return inv_mtr;
}


