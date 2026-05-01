add_library(Config
        src/config_reader.cpp
)
# Публичные include директории
target_include_directories(Config PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Приватные include директории
target_include_directories(Config PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


# Вариант 1: nlohmann_json (самый популярный)
find_package(nlohmann_json REQUIRED)
target_link_libraries(Config PRIVATE nlohmann_json::nlohmann_json Interface)

# Настройка стандарта C++
target_compile_features(Config PRIVATE cxx_std_20)