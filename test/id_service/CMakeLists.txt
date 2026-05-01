add_executable(id_service_test id_service_test.cpp)

target_link_libraries(id_service_test PRIVATE
        IDService                    # основная библиотека
        Catch2::Catch2WithMain      # Catch2
        Utility
        Interface
        Config
)

# ПРАВИЛЬНЫЕ ПУТИ:
target_include_directories(id_service_test PRIVATE

        ${CMAKE_SOURCE_DIR}../../id_service
        ${CMAKE_SOURCE_DIR}../../config_reader

)

add_test(NAME id_service_tests COMMAND id_service_test)