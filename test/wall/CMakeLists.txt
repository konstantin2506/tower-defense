add_executable(wall_test wall_test.cpp)

target_link_libraries(wall_test PRIVATE
        Wall
        Grid
        WorldMap
        Economy
        Config
        Utility
        Interface
        Components
        IDService
        Catch2::Catch2WithMain
)

target_include_directories(wall_test PRIVATE
        ${CMAKE_SOURCE_DIR}../../components
        ${CMAKE_SOURCE_DIR}../../wall
        ${CMAKE_SOURCE_DIR}../../economy
        ${CMAKE_SOURCE_DIR}../../config_reader
        ${CMAKE_SOURCE_DIR}../../utility
        ${CMAKE_SOURCE_DIR}../../interface
        ${CMAKE_SOURCE_DIR}../../grid
        ${CMAKE_SOURCE_DIR}../../world_map
        ${CMAKE_SOURCE_DIR}../../id_service
)

add_test(NAME wall_test COMMAND wall_test)