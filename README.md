# Lab 3 - Matrix Calculator (C++17), individual variant 7

Author: A. I. Ilyushina, BMSTU IU5-14, subgroup 4.

## Tasks

- Create/free dynamic `double**` matrices.
- Add and multiply matrices with dimension checks.
- Create a matrix from a one-dimensional array and print with alignment.
- **Variant 7:** find the minimum value `double matrix_min(Matrix m)`.
- Test typical, boundary, and invalid cases; generate Doxygen documentation.
- Build a Docker image, run GitHub Actions CI, check memory with Valgrind.

## Structure

- `include/matrix.h` and `src/matrix.cpp`: computational functions.
- `include/matrix_io.h` and `src/matrix_io.cpp`: input, formatted output, demo.
- `src/main.cpp`: entry point only.
- `tests/test_matrix.cpp`: Google Test suite (16 tests).
- `tests/test_matrix_offline.cpp`: 16 offline checks with CTest.
- `.github/workflows/ci.yml`: automated build, tests, Valgrind, Doxygen, Docker.

## Build and run in WSL / Ubuntu

```bash
sudo apt-get update
sudo apt-get install -y g++ cmake make libgtest-dev doxygen graphviz valgrind
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DMATRIX_REQUIRE_GTEST=ON
cmake --build build --parallel 2
ctest --test-dir build --verbose
./build/matrix_calculator
```

If `libgtest-dev` is not installed or Internet is unavailable, omit `-DMATRIX_REQUIRE_GTEST=ON`: the project still builds and tests offline using standalone tests. After installing Google Test, reconfigure the build folder to include the Google Test suite.

### Example console input (each matrix has two dimension values then its elements)

```text
2 2
1 2 3 4
2 2
5 6 7 8
```

Expected: `A+B = [[6,8],[10,12]]`, `A*B = [[19,22],[43,50]]`, `min(A) = 1`, `min(B) = 5`.

To capture the output directly without typing:

```bash
printf '2 2\n1 2 3 4\n2 2\n5 6 7 8\n' | ./build/matrix_calculator
```

## Tests and edge cases

Tests cover zero initialization, 1x1 matrix, invalid dimensions, source array copying, sum/product, rectangular product, dimension mismatch, positive and negative minimum values, one-element minimum, invalid/null matrices, `nullptr`, and invalid console input.

## Error handling

All invalid matrix sizes / dimension incompatibilities / null input pointers throw `std::invalid_argument`; failed allocations throw `std::bad_alloc`; malformed user input throws `std::runtime_error` and yields a nonzero exit code. The upper bound of 4 million elements prevents unreasonable allocation sizes. Input matrices and operation results are manually freed after use. `free_matrix(Matrix m)` matches the exact assignment signature, so it takes a shallow copy and **does not reset the caller's pointer**: free any allocated matrix exactly once; never use it afterwards.

## Documentation

```bash
doxygen Doxyfile
```

Open `docs/html/index.html` in your browser (Graphviz helps to produce call graphs).

## Memory check

```bash
valgrind --leak-check=full --error-exitcode=1 ./build/tests/matrix_tests
```

If Google Test isn't installed, check the console application:

```bash
printf '2 2\n1 2 3 4\n2 2\n5 6 7 8\n' | valgrind --leak-check=full --error-exitcode=1 ./build/matrix_calculator
```

## Docker in WSL (requires a working Docker Engine / Docker Desktop integration)

```bash
docker build -t matrix-calculator:lab3 .
printf '2 2\n1 2 3 4\n2 2\n5 6 7 8\n' | docker run -i --rm matrix-calculator:lab3
```

## CI (GitHub Actions)

Create a GitHub repository and push the project's files. On each push or pull request, the workflow builds the program, requires Google Test, executes CTest, runs Valgrind, generates Doxygen HTML, builds Docker image and checks one demo case.

## Formatting and compilation flags

C++17; `-Wall -Wextra -Wpedantic -Werror`; own `matrix_calc` namespace; `.clang-format` (Google-based); `.gitignore` excludes artifacts. The format can be applied with `find include src tests -name '*.cpp' -o -name '*.h' | xargs clang-format -i`.

## AI workflow and report

A prompt used in preparing the code: "Implement a C++17 library with Matrix {double** data; int rows; int cols;}, create/free/add/multiply/from-array/print and matrix_min for variant 7. Use robust error handling, separate .h/.cpp files, Doxygen documentation, CMake, unit tests, Valgrind, CI and Docker."

In the report, include screenshots of **your actual** VS Code/WSL runs, GitHub Actions, Docker and Doxygen. See `evidence/README.md`.
