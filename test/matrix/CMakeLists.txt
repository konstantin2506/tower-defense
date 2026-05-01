add_executable(matrix_test matrix_test.cpp)

target_link_libraries(matrix_test PRIVATE
        Utility

        Catch2::Catch2WithMain
)

target_include_directories(matrix_test PRIVATE
        ${CMAKE_SOURCE_DIR}../../utility
        ${CMAKE_SOURCE_DIR}../../matrix
)

add_test(NAME matrix_test COMMAND matrix_test)