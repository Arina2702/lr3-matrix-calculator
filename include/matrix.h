#pragma once

/**
 * @file matrix.h
 * @brief Dynamic matrix library for C++17.
 * @author A. I. Ilyushina
 */

namespace matrix_calc {

/** @brief Matrix with separately allocated rows. */
struct Matrix {
    double** data = nullptr;  ///< Dynamically allocated rows.
    int rows = 0;             ///< Number of rows.
    int cols = 0;             ///< Number of columns.
};

/**
 * @brief Create a zero-initialized matrix.
 * @param rows Number of rows (positive).
 * @param cols Number of columns (positive).
 * @return A newly allocated matrix; the caller must call free_matrix once.
 * @throws std::invalid_argument For invalid/excessive dimensions.
 * @throws std::bad_alloc If allocation fails.
 */
Matrix create_matrix(int rows, int cols);

/**
 * @brief Free the allocated rows and row-pointer array.
 * @param matrix Matrix to destroy; a nullptr matrix is safe.
 * @note Passed by value to match the assignment. This does not reset the caller's copy;
 *       never call twice for the same allocated matrix.
 */
void free_matrix(Matrix matrix) noexcept;

/**
 * @brief Add two equally sized matrices.
 * @param a First valid matrix.
 * @param b Second valid matrix.
 * @return New matrix with element-wise sums.
 * @throws std::invalid_argument For invalid/incompatible operands.
 */
Matrix matrix_add(Matrix a, Matrix b);

/**
 * @brief Multiply matrices using the i-k-j loop order.
 * @param a Left operand (M by N).
 * @param b Right operand (N by K).
 * @return New M by K matrix.
 * @throws std::invalid_argument For invalid/incompatible operands.
 */
Matrix matrix_multiply(Matrix a, Matrix b);

/**
 * @brief Copy values from a row-major array into a new matrix.
 * @param values Non-null array containing rows * cols doubles.
 * @param rows Number of rows.
 * @param cols Number of columns.
 * @return Independently allocated matrix.
 * @throws std::invalid_argument For invalid values or dimensions.
 */
Matrix matrix_from_array(const double* values, int rows, int cols);

/**
 * @brief Find the minimum element in a matrix (individual task 7).
 * @param matrix Valid nonempty matrix.
 * @return Minimum among all matrix elements.
 * @throws std::invalid_argument For a null or invalid matrix.
 */
double matrix_min(Matrix matrix);

}  // namespace matrix_calc
