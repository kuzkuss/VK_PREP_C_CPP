#include <cmath>
#include <iomanip>
#include <limits>

#include "matrix.h"
#include "exceptions.h"

#define EPS 1e-07

namespace prep {
    Matrix::Matrix(size_t num_rows, size_t num_cols): rows(num_rows), columns(num_cols) {
        elements.reserve(rows * columns);
    }
    Matrix::Matrix(std::istream& is) {
        size_t r = 0;
        size_t c = 0;

        if (!(is >> r) || r <= 0) {
            throw InvalidMatrixStream();
        }
        if (!(is >> c) || c <= 0) {
            throw InvalidMatrixStream();
        }

	rows = r;
	columns = c;

        elements.reserve(rows * columns);

        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < columns; ++j)
                if (!(is >> elements[i * columns + j])) {
                    throw InvalidMatrixStream();
                }
    }
    size_t Matrix::getRows() const {
        return rows;
    }
    size_t Matrix::getCols() const {
        return columns;
    }

    double Matrix::operator()(size_t i, size_t j) const {
        if (i >= rows || j >= columns)
            throw OutOfRange(i, j, *this);
        return elements[i * columns + j];
    }

    double& Matrix::operator()(size_t i, size_t j) {
        if (i >= rows || j >= columns)
            throw OutOfRange(i, j, *this);
        return elements[i * columns + j];
    }

    bool Matrix::operator==(const Matrix& rhs) const {
        bool res = true;

        if (rows != rhs.rows || columns != rhs.columns)
            res = false;

        for (size_t i = 0; i < rows && res; i++) {
            for (size_t j = 0; j < columns && res; j++) {
                if (fabs(rhs.elements[i * columns + j] - elements[i * columns + j]) > EPS) {
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
        if (rows != rhs.rows || columns != rhs.columns)
            throw DimensionMismatch(*this, rhs);

        Matrix res_mtr(rows, columns);

        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < columns; ++j)
                res_mtr.elements[i * columns + j] = elements[i * columns + j] + rhs.elements[i * columns + j];

        return res_mtr;
    }
    Matrix Matrix::operator-(const Matrix& rhs) const {
        if (rows != rhs.rows || columns != rhs.columns)
            throw DimensionMismatch(*this, rhs);

        Matrix res_mtr(rows, columns);

        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < columns; ++j)
                res_mtr.elements[i * columns + j] = elements[i * columns + j] - rhs.elements[i * columns + j];

        return res_mtr;
    }
    Matrix Matrix::operator*(const Matrix& rhs) const {
        if (columns != rhs.rows)
            throw DimensionMismatch(*this, rhs);

        Matrix res_mtr(rows, rhs.columns);

        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < columns; ++j)
                for (size_t k = 0; k < rhs.columns; ++k)
                    res_mtr.elements[i * rhs.columns + k] += elements[i * columns + j] * rhs.elements[j * rhs.columns + k];
        return res_mtr;
    }

    Matrix Matrix::operator*(double val) const {
        Matrix new_mtr(rows, columns);

        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < columns; ++j)
                new_mtr.elements[i * columns + j] = val * elements[i * columns + j];

        return new_mtr;
    }

    Matrix Matrix::transp() const {
        Matrix transp_mtr(columns, rows);

        for (size_t i = 0; i < columns; ++i)
            for (size_t j = 0; j < rows; ++j)
                transp_mtr.elements[i * rows + j] = elements[j * columns + i];
        return transp_mtr;
    }

    static void fill_new_matrix(const Matrix *matrix, Matrix *new_matrix, size_t n, size_t del_col, size_t del_row) {
        size_t new_i = 0;
        size_t new_j = 0;

        for (size_t i = 0; i < n; ++i)
            for (size_t j = 0; j < n; ++j) {
                if (j != del_col && i != del_row) {
                    new_matrix->elements[new_i * new_matrix->columns + new_j] = elements[i * columns + j];
                    if (++new_j == n - 1) {
                        new_j = 0;
                        ++new_i;
                    }
                }
            }
    }

    double Matrix::det() const {
        if (rows != columns)
            throw DimensionMismatch(*this);

        if (rows == 1) {
            return elements[0];
        }

        Matrix new_mtr(rows - 1, columns - 1);

        double determinant = 0.0;
        for (size_t i = 0; i < rows; ++i) {
            fill_new_matrix(&new_mtr, rows, i, 0);
            double cur_det = new_mtr.det();
            determinant += elements[i] * (double((i + 3) % 2) - double((i + 2) % 2)) * cur_det;
        }

        return determinant;
    }

    Matrix Matrix::adj() const {
        if (rows != columns)
            throw DimensionMismatch(*this);
        if (rows == 1)
            throw DimensionMismatch(*this);

        Matrix adj_mtr = transp();

        Matrix new_mtr(rows - 1, columns - 1);

        for (size_t i = 0; i < columns; ++i)
            for (size_t j = 0; j < rows; ++j) {
                fill_new_matrix(&new_mtr, rows, i, j);
                double res = new_mtr.det();
                adj_mtr.elements[i * columns + j] = (double((i + j + 3) % 2) - double((i + j + 2) % 2)) * res;
            }
        return adj_mtr;
    }

    Matrix Matrix::inv() const {
        if (rows != columns)
            throw DimensionMismatch(*this);

        double det_mtr = det();

        if (det_mtr == 0)
            throw SingularMatrix();

        if (rows == 1) {
            Matrix adj_mtr(1, 1);
            adj_mtr.elements[0] = 1 / det_mtr;
            return adj_mtr;
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
                os << std::setprecision(std::numeric_limits<double>::max_digits10)
                   << matrix.elements[i * matrix.columns + j];
                if (i < matrix.rows - 1 || j < matrix.columns - 1)
                    os << ' ';
            }
            os << std::endl;
        }
        return os;
    }
}  // namespace prep

