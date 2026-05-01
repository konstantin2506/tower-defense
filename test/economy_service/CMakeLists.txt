add_executable(economy_service_test economy_service_test.cpp)

target_link_libraries(economy_service_test PRIVATE
        Economy
        Config
        Utility
        Interface
        Components
        Catch2::Catch2WithMain
)

target_include_directories(economy_service_test PRIVATE
        ${CMAKE_SOURCE_DIR}../../economy
        ${CMAKE_SOURCE_DIR}../../config_reader
        ${CMAKE_SOURCE_DIR}../../utility
        ${CMAKE_SOURCE_DIR}../../interface
        ${CMAKE_SOURCE_DIR}../../components
)

add_test(NAME economy_service_test COMMAND economy_service_test)