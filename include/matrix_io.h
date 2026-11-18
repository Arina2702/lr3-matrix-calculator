#pragma once

#include "matrix.h"

#include <iosfwd>
#include <string>

namespace matrix_calc {

/**
 * @brief Read a matrix from a stream, validating all input values.
 * @param in Input stream.
 * @param out Output stream for prompts.
 * @param label Display name for the matrix.
 * @return A newly allocated matrix.
 * @throws std::runtime_error For missing or nonnumeric input.
 * @throws std::invalid_argument For invalid matrix dimensions.
 */
Matrix read_matrix(std::istream& in, std::ostream& out, const std::string& label);

/**
 * @brief Print a matrix with aligned numbers and abbreviated large dimensions.
 * @param out Output stream.
 * @param matrix Matrix to print; null matrix prints an empty marker.
 */
void print_matrix(std::ostream& out, Matrix matrix);

/**
 * @brief Run the console demo and handle errors and memory cleanup.
 * @param in Input stream.
 * @param out Output stream.
 * @param err Error stream.
 * @return Zero on success, one on input/other errors.
 */
int run_calculator(std::istream& in, std::ostream& out, std::ostream& err);

}  // namespace matrix_calc
