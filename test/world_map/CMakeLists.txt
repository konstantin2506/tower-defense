add_executable(world_map_test world_map_test.cpp)

target_link_libraries(world_map_test PRIVATE
        WorldMap
        Config
        Grid
        Catch2::Catch2WithMain
        Utility
        Interface
)

target_include_directories(world_map_test PRIVATE
        ${CMAKE_SOURCE_DIR}../../world_map
        ${CMAKE_SOURCE_DIR}../../config_reader
        ${CMAKE_SOURCE_DIR}../../grid
        ${CMAKE_SOURCE_DIR}../../utility
        ${CMAKE_SOURCE_DIR}../../interface
)

add_test(NAME world_map_tests COMMAND world_map_test)