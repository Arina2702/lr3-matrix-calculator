#include "matrix_io.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

namespace matrix_calc {
namespace {
constexpr int kRowsToShow = 10;
constexpr int kColsToShow = 8;
constexpr int kNumberWidth = 10;

void show_optional_operation(std::ostream& out, Matrix a, Matrix b, bool multiply) {
    try {
        Matrix result = multiply ? matrix_multiply(a, b) : matrix_add(a, b);
        out << (multiply ? "A * B:\n" : "A + B:\n");
        print_matrix(out, result);
        free_matrix(result);
    } catch (const std::invalid_argument& ex) {
        out << (multiply ? "A * B" : "A + B") << ": " << ex.what() << '\n';
    }
}
}  // namespace

Matrix read_matrix(std::istream& in, std::ostream& out, const std::string& label) {
    out << "Enter rows and columns for " << label << ": ";
    int rows = 0;
    int cols = 0;
    if (!(in >> rows >> cols)) {
        throw std::runtime_error("Failed to read matrix dimensions");
    }
    Matrix result = create_matrix(rows, cols);
    out << "Enter " << rows * cols << " values for " << label << " by rows:\n";
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (!(in >> result.data[row][col])) {
                free_matrix(result);
                throw std::runtime_error("Failed to read a matrix element");
            }
        }
    }
    return result;
}

void print_matrix(std::ostream& out, Matrix matrix) {
    if (matrix.data == nullptr || matrix.rows <= 0 || matrix.cols <= 0) {
        out << "[Empty matrix]\n";
        return;
    }
    const auto old_flags = out.flags();
    const auto old_precision = out.precision();
    out << std::fixed << std::setprecision(2);
    for (int row = 0; row < matrix.rows; ++row) {
        if (matrix.rows > kRowsToShow && row == kRowsToShow / 2) {
            out << "[ ... skipped " << matrix.rows - kRowsToShow << " rows ... ]\n";
            row = matrix.rows - kRowsToShow / 2;
        }
        out << "[";
        for (int col = 0; col < matrix.cols; ++col) {
            if (matrix.cols > kColsToShow && col == kColsToShow / 2) {
                out << " ... ";
                col = matrix.cols - kColsToShow / 2;
            }
            out << std::setw(kNumberWidth) << matrix.data[row][col];
        }
        out << " ]\n";
    }
    if (matrix.rows > kRowsToShow || matrix.cols > kColsToShow) {
        out << "Matrix size: " << matrix.rows << " x " << matrix.cols << '\n';
    }
    out.flags(old_flags);
    out.precision(old_precision);
}

int run_calculator(std::istream& in, std::ostream& out, std::ostream& err) {
    Matrix a{};
    Matrix b{};
    int status = 0;
    try {
        a = read_matrix(in, out, "A");
        b = read_matrix(in, out, "B");
        out << "Matrix A:\n";
        print_matrix(out, a);
        out << "Matrix B:\n";
        print_matrix(out, b);
        show_optional_operation(out, a, b, false);
        show_optional_operation(out, a, b, true);
        out << "min(A) = " << matrix_min(a) << '\n';
        out << "min(B) = " << matrix_min(b) << '\n';
    } catch (const std::exception& ex) {
        err << "Error: " << ex.what() << '\n';
        status = 1;
    }
    free_matrix(a);
    free_matrix(b);
    return status;
}

}  // namespace matrix_calc
