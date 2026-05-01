# Библиотека для модуля utility (общие типы и компоненты)
add_library(Utility INTERFACE
)

# Публичные include директории
target_include_directories(Utility INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)



# Зависимости от других модулей
target_link_libraries(Utility INTERFACE
        Interface
)

# Настройка стандарта C++
target_compile_features(Utility INTERFACE cxx_std_20)
