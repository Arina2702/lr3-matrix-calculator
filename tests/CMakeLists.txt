# Self-contained tests always work offline.
add_executable(matrix_tests_offline test_matrix_offline.cpp)
target_link_libraries(matrix_tests_offline PRIVATE matrix_core)
target_compile_options(matrix_tests_offline PRIVATE -Wall -Wextra -Wpedantic -Werror)

set(MATRIX_OFFLINE_CASES
    create_zero create_minimum create_bad_dimensions
    from_array add multiply multiply_rectangular
    add_mismatch multiply_mismatch min_positive min_negative
    min_single min_invalid null_source null_free input_invalid)
foreach(case_name IN LISTS MATRIX_OFFLINE_CASES)
    add_test(NAME "offline_${case_name}" COMMAND matrix_tests_offline "${case_name}")
endforeach()

find_package(GTest QUIET)
if(GTest_FOUND)
    add_executable(matrix_tests test_matrix.cpp)
    target_link_libraries(matrix_tests PRIVATE matrix_core GTest::gtest_main)
    target_compile_options(matrix_tests PRIVATE -Wall -Wextra -Wpedantic -Werror)
    include(GoogleTest)
    gtest_discover_tests(matrix_tests)
elseif(MATRIX_REQUIRE_GTEST)
    message(FATAL_ERROR "Google Test not found; install libgtest-dev")
else()
    message(STATUS "Google Test not found: offline tests enabled. To add Google Test, install libgtest-dev and reconfigure.")
endif()
