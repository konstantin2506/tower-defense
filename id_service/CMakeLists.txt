# Библиотека для модуля components
add_library(IDService INTERFACE
)

# Публичные include директории
target_include_directories(IDService INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# Зависимости от других модулей
target_link_libraries(IDService INTERFACE
        Utility
        Interface
)

# Настройка стандарта C++
target_compile_features(IDService INTERFACE cxx_std_20)
