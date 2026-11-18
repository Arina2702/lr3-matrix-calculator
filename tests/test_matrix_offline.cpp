#include "matrix.h"
#include "matrix_io.h"

#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {
using matrix_calc::Matrix;
using matrix_calc::create_matrix;
using matrix_calc::free_matrix;
using matrix_calc::matrix_add;
using matrix_calc::matrix_from_array;
using matrix_calc::matrix_min;
using matrix_calc::matrix_multiply;

void require(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

template <typename Function>
void requires_invalid_argument(Function operation) {
    bool thrown = false;
    try { operation(); } catch (const std::invalid_argument&) { thrown = true; }
    require(thrown, "Expected std::invalid_argument");
}

void run_case(const std::string& name) {
    if (name == "create_zero") {
        Matrix a = create_matrix(2, 3);
        require(a.rows == 2 && a.cols == 3, "Wrong dimensions");
        for (int i = 0; i < a.rows; ++i)
            for (int j = 0; j < a.cols; ++j)
                require(a.data[i][j] == 0.0, "Element is not zero");
        free_matrix(a);
    } else if (name == "create_minimum") {
        Matrix a = create_matrix(1, 1);
        require(a.data[0][0] == 0.0, "1x1 not initialized");
        free_matrix(a);
    } else if (name == "create_bad_dimensions") {
        requires_invalid_argument([] { (void)create_matrix(0, 3); });
        requires_invalid_argument([] { (void)create_matrix(3, -1); });
    } else if (name == "from_array") {
        const double raw[] = {1, 2, 3, 4, 5, 6};
        Matrix a = matrix_from_array(raw, 2, 3);
        require(a.data[0][2] == 3.0 && a.data[1][0] == 4.0, "Wrong array mapping");
        free_matrix(a);
    } else if (name == "add") {
        const double raw_a[] = {1, 2, 3, 4};
        const double raw_b[] = {5, 6, 7, 8};
        Matrix a = matrix_from_array(raw_a, 2, 2), b = matrix_from_array(raw_b, 2, 2);
        Matrix c = matrix_add(a, b);
        require(c.data[0][0] == 6.0 && c.data[1][1] == 12.0, "Bad sum");
        free_matrix(a); free_matrix(b); free_matrix(c);
    } else if (name == "multiply") {
        const double raw_a[] = {1, 2, 3, 4};
        const double raw_b[] = {5, 6, 7, 8};
        Matrix a = matrix_from_array(raw_a, 2, 2), b = matrix_from_array(raw_b, 2, 2);
        Matrix c = matrix_multiply(a, b);
        require(c.data[0][0] == 19.0 && c.data[1][1] == 50.0, "Bad product");
        free_matrix(a); free_matrix(b); free_matrix(c);
    } else if (name == "multiply_rectangular") {
        const double raw_a[] = {1, 2, 3};
        const double raw_b[] = {4, 5, 6};
        Matrix a = matrix_from_array(raw_a, 1, 3), b = matrix_from_array(raw_b, 3, 1);
        Matrix c = matrix_multiply(a, b);
        require(c.rows == 1 && c.cols == 1 && c.data[0][0] == 32.0, "Bad rectangular product");
        free_matrix(a); free_matrix(b); free_matrix(c);
    } else if (name == "add_mismatch") {
        Matrix a = create_matrix(2, 2), b = create_matrix(2, 3);
        requires_invalid_argument([&] { (void)matrix_add(a, b); });
        free_matrix(a); free_matrix(b);
    } else if (name == "multiply_mismatch") {
        Matrix a = create_matrix(2, 2), b = create_matrix(3, 2);
        requires_invalid_argument([&] { (void)matrix_multiply(a, b); });
        free_matrix(a); free_matrix(b);
    } else if (name == "min_positive") {
        const double raw[] = {5, 8, 2, 9};
        Matrix a = matrix_from_array(raw, 2, 2);
        require(matrix_min(a) == 2.0, "Bad minimum positive");
        free_matrix(a);
    } else if (name == "min_negative") {
        const double raw[] = {-3, 7, -10, 1};
        Matrix a = matrix_from_array(raw, 2, 2);
        require(matrix_min(a) == -10.0, "Bad minimum negative");
        free_matrix(a);
    } else if (name == "min_single") {
        Matrix a = create_matrix(1, 1);
        a.data[0][0] = -4.25;
        require(matrix_min(a) == -4.25, "Bad single minimum");
        free_matrix(a);
    } else if (name == "min_invalid") {
        requires_invalid_argument([] { (void)matrix_min(Matrix{}); });
    } else if (name == "null_source") {
        requires_invalid_argument([] { (void)matrix_from_array(nullptr, 2, 3); });
    } else if (name == "null_free") {
        free_matrix(Matrix{});
        free_matrix(Matrix{});
    } else if (name == "input_invalid") {
        std::istringstream in("2 x");
        std::ostringstream out, err;
        require(matrix_calc::run_calculator(in, out, err) == 1, "Invalid input was accepted");
        require(err.str().find("Error:") != std::string::npos, "No error printed");
    } else {
        throw std::runtime_error("Unknown test name: " + name);
    }
}
}  // namespace

int main(int argc, char** argv) {
    if (argc != 2) return 2;
    try {
        run_case(argv[1]);
        std::cout << "PASS " << argv[1] << '\n';
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "FAIL " << argv[1] << ": " << error.what() << '\n';
        return 1;
    }
}
