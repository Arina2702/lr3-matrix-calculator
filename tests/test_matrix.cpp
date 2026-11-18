#include "matrix.h"
#include "matrix_io.h"

#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>

using matrix_calc::Matrix;
using matrix_calc::create_matrix;
using matrix_calc::free_matrix;
using matrix_calc::matrix_add;
using matrix_calc::matrix_from_array;
using matrix_calc::matrix_min;
using matrix_calc::matrix_multiply;

TEST(MatrixTest, CreateZeroInitialized) {
    Matrix a = create_matrix(3, 4);
    EXPECT_EQ(a.rows, 3);
    EXPECT_EQ(a.cols, 4);
    for (int i = 0; i < a.rows; ++i)
        for (int j = 0; j < a.cols; ++j)
            EXPECT_DOUBLE_EQ(a.data[i][j], 0.0);
    free_matrix(a);
}

TEST(MatrixTest, CreateMinimumDimensions) {
    Matrix a = create_matrix(1, 1);
    EXPECT_DOUBLE_EQ(a.data[0][0], 0.0);
    free_matrix(a);
}

TEST(MatrixTest, RejectInvalidDimensions) {
    EXPECT_THROW(create_matrix(0, 3), std::invalid_argument);
    EXPECT_THROW(create_matrix(3, -1), std::invalid_argument);
    EXPECT_THROW(create_matrix(5000, 5000), std::invalid_argument);
}

TEST(MatrixTest, FromArray) {
    const double data[] = {1, 2, 3, 4, 5, 6};
    Matrix a = matrix_from_array(data, 2, 3);
    EXPECT_DOUBLE_EQ(a.data[1][0], 4);
    EXPECT_DOUBLE_EQ(a.data[0][2], 3);
    free_matrix(a);
}

TEST(MatrixTest, ArrayInputCannotBeNull) {
    EXPECT_THROW(matrix_from_array(nullptr, 2, 2), std::invalid_argument);
}

TEST(MatrixTest, Addition) {
    const double first[] = {1, 2, 3, 4};
    const double second[] = {5, 6, 7, 8};
    Matrix a = matrix_from_array(first, 2, 2), b = matrix_from_array(second, 2, 2);
    Matrix c = matrix_add(a, b);
    EXPECT_DOUBLE_EQ(c.data[0][0], 6);
    EXPECT_DOUBLE_EQ(c.data[0][1], 8);
    EXPECT_DOUBLE_EQ(c.data[1][0], 10);
    EXPECT_DOUBLE_EQ(c.data[1][1], 12);
    free_matrix(a); free_matrix(b); free_matrix(c);
}

TEST(MatrixTest, Multiplication) {
    const double first[] = {1, 2, 3, 4};
    const double second[] = {5, 6, 7, 8};
    Matrix a = matrix_from_array(first, 2, 2), b = matrix_from_array(second, 2, 2);
    Matrix c = matrix_multiply(a, b);
    EXPECT_DOUBLE_EQ(c.data[0][0], 19);
    EXPECT_DOUBLE_EQ(c.data[0][1], 22);
    EXPECT_DOUBLE_EQ(c.data[1][0], 43);
    EXPECT_DOUBLE_EQ(c.data[1][1], 50);
    free_matrix(a); free_matrix(b); free_matrix(c);
}

TEST(MatrixTest, MultiplyRectangularMatrices) {
    const double first[] = {1, 2, 3};
    const double second[] = {4, 5, 6};
    Matrix a = matrix_from_array(first, 1, 3), b = matrix_from_array(second, 3, 1);
    Matrix c = matrix_multiply(a, b);
    EXPECT_EQ(c.rows, 1);
    EXPECT_EQ(c.cols, 1);
    EXPECT_DOUBLE_EQ(c.data[0][0], 32);
    free_matrix(a); free_matrix(b); free_matrix(c);
}

TEST(MatrixTest, AdditionIncompatible) {
    Matrix a = create_matrix(2, 2), b = create_matrix(2, 3);
    EXPECT_THROW(matrix_add(a, b), std::invalid_argument);
    free_matrix(a); free_matrix(b);
}

TEST(MatrixTest, MultiplicationIncompatible) {
    Matrix a = create_matrix(2, 2), b = create_matrix(3, 2);
    EXPECT_THROW(matrix_multiply(a, b), std::invalid_argument);
    free_matrix(a); free_matrix(b);
}

TEST(MatrixTest, MinWithNegativeElements) {
    const double data[] = {4, -2, 6, -7, 5, 10};
    Matrix a = matrix_from_array(data, 2, 3);
    EXPECT_DOUBLE_EQ(matrix_min(a), -7);
    free_matrix(a);
}

TEST(MatrixTest, MinWithPositiveElements) {
    const double data[] = {5, 8, 2, 9};
    Matrix a = matrix_from_array(data, 2, 2);
    EXPECT_DOUBLE_EQ(matrix_min(a), 2);
    free_matrix(a);
}

TEST(MatrixTest, MinSingleElement) {
    Matrix a = create_matrix(1, 1);
    a.data[0][0] = 0.25;
    EXPECT_DOUBLE_EQ(matrix_min(a), 0.25);
    free_matrix(a);
}

TEST(MatrixTest, MinInvalidMatrix) {
    EXPECT_THROW(matrix_min(Matrix{}), std::invalid_argument);
}

TEST(MatrixTest, FreeNullMatrix) {
    EXPECT_NO_THROW(free_matrix(Matrix{}));
    EXPECT_NO_THROW(free_matrix(Matrix{}));
}

TEST(MatrixTest, InvalidInput) {
    std::istringstream in("x 2");
    std::ostringstream out, err;
    EXPECT_EQ(matrix_calc::run_calculator(in, out, err), 1);
    EXPECT_FALSE(err.str().empty());
}
