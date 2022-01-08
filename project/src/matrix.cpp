#include <cmath>
#include <functional>
#include <iomanip>
#include <limits>

#include "exceptions.h"

#include "matrix.h"

static constexpr double kEps = 1e-07;

static constexpr double precision = std::numeric_limits<double>::max_digits10;

static void fill_new_matrix(const prep::Matrix &src_matrix, prep::Matrix *new_matrix,
                            size_t skip_col, size_t skip_row);
template <typename T>
static prep::Matrix arithmetic_operation(const prep::Matrix& lhs,
                    const prep::Matrix& rhs, T oper);

namespace prep {
    Matrix::Matrix(std::istream& is) {
        size_t input_rows = 0;
        size_t input_cols = 0;

        if (!(is >> input_rows) || input_rows == 0) {
            throw InvalidMatrixStream();
        }
        if (!(is >> input_cols) || input_cols == 0) {
            throw InvalidMatrixStream();
        }

        rows = input_rows;
        columns = input_cols;
        elements.resize(rows * columns);

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < columns; ++j) {
                if (!(is >> elements[i * columns + j])) {
                    throw InvalidMatrixStream();
                }
            }
        }
    }
    size_t Matrix::getRows() const {
        return rows;
    }
    size_t Matrix::getCols() const {
        return columns;
    }

    double Matrix::operator()(size_t i, size_t j) const {
        if (i >= rows || j >= columns) {
            throw OutOfRange(i, j, *this);
        }
        return elements[i * columns + j];
    }

    double& Matrix::operator()(size_t i, size_t j) {
        if (i >= rows || j >= columns) {
            throw OutOfRange(i, j, *this);
        }
        return elements[i * columns + j];
    }

    bool Matrix::operator==(const Matrix& rhs) const {
        bool res = true;

        if (rows != rhs.rows || columns != rhs.columns) {
            res = false;
        }

        for (size_t i = 0; i < rows && res; i++) {
            for (size_t j = 0; j < columns && res; j++) {
                if (fabs(rhs.elements[i * columns + j] - elements[i * columns + j]) > kEps) {
                    res = false;
                }
            }
        }
        return res;
    }

    bool Matrix::operator!=(const Matrix& rhs) const {
        return !(*this == rhs);
    }

    Matrix Matrix::operator+(const Matrix& rhs) const {
        return arithmetic_operation(*this, rhs, std::plus<double>());
    }

    Matrix Matrix::operator-(const Matrix& rhs) const {
        return arithmetic_operation(*this, rhs, std::minus<double>());
    }

    Matrix Matrix::operator*(const Matrix& rhs) const {
        if (columns != rhs.rows) {
            throw DimensionMismatch(*this, rhs);
        }

        Matrix res_mtr(rows, rhs.columns);

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < columns; ++j) {
                for (size_t k = 0; k < rhs.columns; ++k) {
                    res_mtr.elements[i * rhs.columns + k] +=
                            elements[i * columns + j] * rhs.elements[j * rhs.columns + k];
                }
            }
        }
        return res_mtr;
    }

    Matrix Matrix::operator*(double val) const {
        Matrix new_mtr(rows, columns);

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < columns; ++j) {
                new_mtr.elements[i * columns + j] = val * elements[i * columns + j];
            }
        }

        return new_mtr;
    }

    Matrix Matrix::transp() const {
        Matrix transp_mtr(columns, rows);

        for (size_t i = 0; i < columns; ++i) {
            for (size_t j = 0; j < rows; ++j) {
                transp_mtr.elements[i * rows + j] = elements[j * columns + i];
            }
        }
        return transp_mtr;
    }

    double Matrix::det() const {
        if (rows != columns) {
            throw DimensionMismatch(*this);
        }

        if (rows == 1) {
            return elements[0];
        }

        Matrix new_mtr(rows - 1, columns - 1);

        double determinant = 0.0;
        for (size_t i = 0; i < rows; ++i) {
            fill_new_matrix(*this, &new_mtr, i, 0);
            double cur_det = new_mtr.det();
            if (i % 2 != 0) {
                cur_det = -cur_det;
            }
            determinant += elements[i] * cur_det;
        }

        return determinant;
    }

    Matrix Matrix::adj() const {
        if (rows != columns) {
            throw DimensionMismatch(*this);
        }
        if (rows == 1) {
            throw DimensionMismatch(*this);
        }

        Matrix adj_mtr = transp();

        Matrix new_mtr(rows - 1, columns - 1);

        for (size_t i = 0; i < columns; ++i) {
            for (size_t j = 0; j < rows; ++j) {
                fill_new_matrix(*this, &new_mtr, i, j);
                double res = new_mtr.det();
                if ((i + j) % 2 != 0) {
                    res = -res;
                }
                adj_mtr.elements[i * columns + j] = res;
            }
        }
        return adj_mtr;
    }

    Matrix Matrix::inv() const {
        if (rows != columns) {
            throw DimensionMismatch(*this);
        }

        double det_mtr = det();

        if (fabs(det_mtr - 0.0) < kEps) {
            throw SingularMatrix();
        }

        if (rows == 1) {
            Matrix inv_mtr(1, 1);
            inv_mtr.elements[0] = 1 / det_mtr;
            return inv_mtr;
        }

        Matrix adj_mtr = adj();

        Matrix inv_mtr = adj_mtr * (1 / det_mtr);

        return inv_mtr;
    }

    Matrix operator*(double val, const Matrix& matrix) {
        return matrix * val;
    }

    std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
        os << matrix.rows << ' ' << matrix.columns << std::endl;
        for (size_t i = 0; i < matrix.rows; i++) {
            for (size_t j = 0; j < matrix.columns; j++) {
                os << std::setprecision(precision)
                   << matrix.elements[i * matrix.columns + j];
                if (i < matrix.rows - 1 || j < matrix.columns - 1) {
                    os << ' ';
                }
            }
            os << std::endl;
        }
        return os;
    }
}  // namespace prep

template <typename T>
static prep::Matrix arithmetic_operation(const prep::Matrix& lhs,
            const prep::Matrix& rhs, T oper) {
    size_t l_rows = lhs.getRows();
    size_t l_cols = lhs.getCols();
    size_t r_rows = rhs.getRows();
    size_t r_cols = rhs.getCols();

    if (l_rows != r_rows || l_cols != r_cols) {
        throw prep::DimensionMismatch(lhs, rhs);
    }

    prep::Matrix res_mtr(l_rows, l_cols);

    for (size_t i = 0; i < l_rows; ++i) {
        for (size_t j = 0; j < l_cols; ++j) {
            res_mtr(i, j) = oper(lhs(i, j), rhs(i, j));
        }
    }

    return res_mtr;
}

static void fill_new_matrix(const prep::Matrix &src_matrix,
                    prep::Matrix *new_matrix, size_t skip_col, size_t skip_row) {
    size_t new_matrix_row = 0;
    size_t new_matrix_col = 0;

    size_t src_matrix_rows = src_matrix.getRows();

    for (size_t i = 0; i < src_matrix_rows; ++i) {
        for (size_t j = 0; j < src_matrix_rows; ++j) {
            if (j == skip_col || i == skip_row) {
                continue;
            }
            (*new_matrix)(new_matrix_row, new_matrix_col) = src_matrix(i, j);
            ++new_matrix_col;
        }
        if (new_matrix_col == src_matrix_rows - 1) {
            new_matrix_col = 0;
            ++new_matrix_row;
        }
    }
}

