add_executable(castle_test castle_test.cpp)

target_link_libraries(castle_test PRIVATE
        Castle
        Economy
        Config
        Utility
        Interface
        Components
        IDService
        Catch2::Catch2WithMain
)

target_include_directories(castle_test PRIVATE
        ${CMAKE_SOURCE_DIR}../../components
        ${CMAKE_SOURCE_DIR}../../castle
        ${CMAKE_SOURCE_DIR}../../economy
        ${CMAKE_SOURCE_DIR}../../config_reader
        ${CMAKE_SOURCE_DIR}../../utility
        ${CMAKE_SOURCE_DIR}../../interface

        ${CMAKE_SOURCE_DIR}../../id_service
)

add_test(NAME castle_test COMMAND castle_test)