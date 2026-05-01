add_library(Economy
        src/economy_service.cpp
)

# Публичные include директории
target_include_directories(Economy PUBLIC
        include
)

# Приватные include директории
target_include_directories(Economy PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Если экономика зависит от других модулей (например, общие типы)
target_link_libraries(Economy PRIVATE
        Utility
        Interface
        )

# Настройка стандарта C++
target_compile_features(Economy PRIVATE cxx_std_20)