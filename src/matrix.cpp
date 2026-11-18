#include "matrix.h"

#include <cstddef>
#include <stdexcept>

namespace matrix_calc {
namespace {
constexpr std::size_t kMaxElements = 4'000'000;

void validate_matrix(Matrix matrix) {
    if (matrix.data == nullptr || matrix.rows <= 0 || matrix.cols <= 0) {
        throw std::invalid_argument("Matrix is null or has invalid dimensions");
    }
    for (int row = 0; row < matrix.rows; ++row) {
        if (matrix.data[row] == nullptr) {
            throw std::invalid_argument("Matrix contains a null row");
        }
    }
}
}  // namespace

Matrix create_matrix(int rows, int cols) {
    if (rows <= 0 || cols <= 0 ||
        static_cast<std::size_t>(rows) * static_cast<std::size_t>(cols) > kMaxElements) {
        throw std::invalid_argument("Dimensions must be positive (at most 4000000 elements)");
    }
    Matrix result{nullptr, rows, cols};
    result.data = new double*[static_cast<std::size_t>(rows)]{};
    try {
        for (int row = 0; row < rows; ++row) {
            result.data[row] = new double[static_cast<std::size_t>(cols)]{};
        }
    } catch (...) {
        free_matrix(result);
        throw;
    }
    return result;
}

void free_matrix(Matrix matrix) noexcept {
    if (matrix.data == nullptr) {
        return;
    }
    for (int row = 0; row < matrix.rows; ++row) {
        delete[] matrix.data[row];
    }
    delete[] matrix.data;
}

Matrix matrix_add(Matrix a, Matrix b) {
    validate_matrix(a);
    validate_matrix(b);
    if (a.rows != b.rows || a.cols != b.cols) {
        throw std::invalid_argument("Addition requires identical matrix dimensions");
    }
    Matrix result = create_matrix(a.rows, a.cols);
    for (int row = 0; row < a.rows; ++row) {
        for (int col = 0; col < a.cols; ++col) {
            result.data[row][col] = a.data[row][col] + b.data[row][col];
        }
    }
    return result;
}

Matrix matrix_multiply(Matrix a, Matrix b) {
    validate_matrix(a);
    validate_matrix(b);
    if (a.cols != b.rows) {
        throw std::invalid_argument("Multiplication requires A.cols == B.rows");
    }
    Matrix result = create_matrix(a.rows, b.cols);
    for (int row = 0; row < a.rows; ++row) {
        for (int inner = 0; inner < a.cols; ++inner) {
            const double term = a.data[row][inner];
            for (int col = 0; col < b.cols; ++col) {
                result.data[row][col] += term * b.data[inner][col];
            }
        }
    }
    return result;
}

Matrix matrix_from_array(const double* values, int rows, int cols) {
    if (values == nullptr) {
        throw std::invalid_argument("Source array cannot be null");
    }
    Matrix result = create_matrix(rows, cols);
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            result.data[row][col] =
                values[static_cast<std::size_t>(row) * static_cast<std::size_t>(cols) +
                       static_cast<std::size_t>(col)];
        }
    }
    return result;
}

double matrix_min(Matrix matrix) {
    validate_matrix(matrix);
    double minimum = matrix.data[0][0];
    for (int row = 0; row < matrix.rows; ++row) {
        for (int col = 0; col < matrix.cols; ++col) {
            if (matrix.data[row][col] < minimum) {
                minimum = matrix.data[row][col];
            }
        }
    }
    return minimum;
}

}  // namespace matrix_calc
