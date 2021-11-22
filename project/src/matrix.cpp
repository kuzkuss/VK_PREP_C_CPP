#include <cmath>
#include <iomanip>
#include <limits>

#include "matrix.h"
#include "exceptions.h"

namespace prep {
    Matrix::Matrix(size_t num_rows, size_t num_cols): rows(num_rows), columns(num_cols) {
        for (size_t i = 0; i < rows; ++i) {
            mtr.emplace_back(std::vector<double>(columns));
        }
    }
    Matrix::Matrix(std::istream& is) {
	int r = 0;
	int c = 0;

        if (!(is >> r) || r <= 0) {
            throw InvalidMatrixStream();
        }
        if (!(is >> c) || c <= 0) {
            throw InvalidMatrixStream();
        }

	rows = r;
	columns = c;

        for (size_t i = 0; i < rows; ++i) {
            mtr.push_back(std::vector<double>(columns));
        }

        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < columns; ++j)
                if (!(is >> mtr[i][j])) {
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
        return mtr[i][j];
    }

    double& Matrix::operator()(size_t i, size_t j) {
        if (i >= rows || j >= columns)
            throw OutOfRange(i, j, *this);
        return mtr[i][j];
    }

    bool Matrix::operator==(const Matrix& rhs) const {
        bool res = true;

        if (rows != rhs.rows || columns != rhs.columns)
            res = false;

        for (size_t i = 0; i < rows && res; i++) {
            for (size_t j = 0; j < columns && res; j++) {
                if (fabs(rhs.mtr[i][j] - mtr[i][j]) > std::numeric_limits<double>::epsilon()) {
                    res = false;
                }
            }
        }
        return res;
    }

    bool Matrix::operator!=(const Matrix& rhs) const {
        bool res = true;

        size_t count_eq_elements = 0;
        if (rows == rhs.rows && columns == rhs.columns) {
            for (size_t i = 0; i < rows; i++) {
                for (size_t j = 0; j < columns; j++) {
                    if (fabs(rhs.mtr[i][j] - mtr[i][j]) <= std::numeric_limits<double>::epsilon()) {
                        count_eq_elements++;
                    }
                }
            }
        }

        if (count_eq_elements == rows * columns)
            res = false;

        return res;
    }

    Matrix Matrix::operator+(const Matrix& rhs) const {
        if (this->rows != rhs.rows || this->columns != rhs.columns)
            throw DimensionMismatch(*this, rhs);

        Matrix res_mtr(this->rows, this->columns);

        for (size_t i = 0; i < this->rows; ++i)
            for (size_t j = 0; j < this->columns; ++j)
                res_mtr.mtr[i][j] = this->mtr[i][j] + rhs.mtr[i][j];

        return res_mtr;
    }
    Matrix Matrix::operator-(const Matrix& rhs) const {
        if (this->rows != rhs.rows || this->columns != rhs.columns)
            throw DimensionMismatch(*this, rhs);

        Matrix res_mtr(this->rows, this->columns);

        for (size_t i = 0; i < this->rows; ++i)
            for (size_t j = 0; j < this->columns; ++j)
                res_mtr.mtr[i][j] = this->mtr[i][j] - rhs.mtr[i][j];

        return res_mtr;
    }
    Matrix Matrix::operator*(const Matrix& rhs) const {
        if (this->columns != rhs.rows)
            throw DimensionMismatch(*this, rhs);

        Matrix res_mtr(this->rows, rhs.columns);

        for (size_t i = 0; i < this->rows; ++i)
            for (size_t j = 0; j < this->columns; ++j)
                for (size_t k = 0; k < rhs.columns; ++k)
                    res_mtr.mtr[i][k] += this->mtr[i][j] * rhs.mtr[j][k];
        return res_mtr;
    }

    Matrix Matrix::operator*(double val) const {
        Matrix new_mtr(this->rows, this->columns);

        for (size_t i = 0; i < this->rows; ++i)
            for (size_t j = 0; j < this->columns; ++j)
                new_mtr.mtr[i][j] = val * this->mtr[i][j];

        return new_mtr;
    }

    Matrix Matrix::transp() const {
        Matrix transp_mtr(this->columns, this->rows);

        for (size_t i = 0; i < this->columns; ++i)
            for (size_t j = 0; j < this->rows; ++j)
                transp_mtr.mtr[i][j] = this->mtr[j][i];
        return transp_mtr;
    }

    void Matrix::fill_new_matrix(Matrix *new_matrix, size_t n, size_t del_col, size_t del_row) const {
        size_t new_i = 0;
        size_t new_j = 0;

        for (size_t i = 0; i < n; ++i)
            for (size_t j = 0; j < n; ++j) {
                if (j != del_col && i != del_row) {
                    new_matrix->mtr[new_i][new_j] = this->mtr[i][j];
                    if (++new_j == n - 1) {
                        new_j = 0;
                        ++new_i;
                    }
                }
            }
    }

    double Matrix::det() const {
        if (this->rows != this->columns)
            throw DimensionMismatch(*this);

        if (this->rows == 1) {
            return this->mtr[0][0];
        }

        Matrix new_mtr(this->rows - 1, this->columns - 1);

        double determinant = 0.0;
        for (size_t i = 0; i < this->rows; ++i) {
            this->fill_new_matrix(&new_mtr, this->rows, i, 0);
            double cur_det = new_mtr.det();
            determinant += this->mtr[0][i] * (double((i + 3) % 2) - double((i + 2) % 2)) * cur_det;
        }

        return determinant;
    }

    Matrix Matrix::adj() const {
        if (this->rows != this->columns)
            throw DimensionMismatch(*this);
        if (this->rows == 1)
            throw DimensionMismatch(*this);

        Matrix adj_mtr = this->transp();

        Matrix new_mtr(this->rows - 1, this->columns - 1);

        for (size_t i = 0; i < this->columns; ++i)
            for (size_t j = 0; j < this->rows; ++j) {
                this->fill_new_matrix(&new_mtr, this->rows, i, j);
                double res = new_mtr.det();
                adj_mtr.mtr[i][j] = (double((i + j + 3) % 2) - double((i + j + 2) % 2)) * res;
                adj_mtr.mtr[i][j] = roundl(adj_mtr.mtr[i][j] * 10000000) / 10000000;
            }
        return adj_mtr;
    }

    Matrix Matrix::inv() const {
        if (this->rows != this->columns)
            throw DimensionMismatch(*this);

        double det_mtr = this->det();

        if (det_mtr == 0)
            throw SingularMatrix();

        if (this->rows == 1) {
            Matrix adj_mtr(1, 1);
            adj_mtr.mtr[0][0] = 1 / det_mtr;
            return adj_mtr;
        }

        Matrix adj_mtr = this->adj();

        Matrix inv_mtr = adj_mtr * (1 / det_mtr);

        return inv_mtr;
    }

    Matrix operator*(double val, const Matrix& matrix) {
        Matrix new_mtr(matrix.rows, matrix.columns);

        for (size_t i = 0; i < matrix.rows; ++i)
            for (size_t j = 0; j < matrix.columns; ++j)
                new_mtr.mtr[i][j] = val * matrix.mtr[i][j];

        return new_mtr;
    }

    std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
        os << matrix.rows << ' ' << matrix.columns << std::endl;
        for (size_t i = 0; i < matrix.rows; i++) {
            for (size_t j = 0; j < matrix.columns; j++) {
                os << std::setprecision(std::numeric_limits<double>::max_digits10)
                   << static_cast<long double>(matrix.mtr[i][j]);
                if (i < matrix.rows - 1 || j < matrix.columns - 1)
                    os << ' ';
            }
            os << std::endl;
        }
        return os;
    }
}  // namespace prep

