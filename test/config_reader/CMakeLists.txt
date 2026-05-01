add_executable(config_reader_test config_reader_test.cpp)

target_link_libraries(config_reader_test PRIVATE
        Config                 # основная библиотека
        Catch2::Catch2WithMain      # Catch2
        Utility
        Interface

)

# ПРАВИЛЬНЫЕ ПУТИ:
target_include_directories(id_service_test PRIVATE

        ${CMAKE_SOURCE_DIR}../../config_reader

)

add_test(NAME config_reader_test COMMAND config_reader_test)