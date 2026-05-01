add_executable(grid_test grid_test.cpp)

target_link_libraries(grid_test PRIVATE
        Grid
        Config
        Utility
        Interface
        Catch2::Catch2WithMain
)

target_include_directories(grid_test PRIVATE
        ${CMAKE_SOURCE_DIR}../../components
        ${CMAKE_SOURCE_DIR}../../config_reader
        ${CMAKE_SOURCE_DIR}../../utility
        ${CMAKE_SOURCE_DIR}../../interface
        ${CMAKE_SOURCE_DIR}../../grid
        ${CMAKE_SOURCE_DIR}../../world_map
)

add_test(NAME grid_test COMMAND grid_test)