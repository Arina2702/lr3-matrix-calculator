cmake_minimum_required(VERSION 3.16)
project(matrix_calculator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MATRIX_ENABLE_TESTS "Build matrix tests" ON)
option(MATRIX_REQUIRE_GTEST "Require installed Google Test" OFF)

add_library(matrix_core STATIC src/matrix.cpp src/matrix_io.cpp)
target_include_directories(matrix_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_options(matrix_core PRIVATE -Wall -Wextra -Wpedantic -Werror)

add_executable(matrix_calculator src/main.cpp)
target_link_libraries(matrix_calculator PRIVATE matrix_core)
target_compile_options(matrix_calculator PRIVATE -Wall -Wextra -Wpedantic -Werror)

if(MATRIX_ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
