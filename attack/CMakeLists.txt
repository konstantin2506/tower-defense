add_library(Attack
        src/tower_attack_strategy.cpp
        src/range_enemy_attack.cpp
        src/air_attack_strategy.cpp
)

# Публичные include директории
target_include_directories(Attack PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Приватные include директории
target_include_directories(Attack PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Зависимости от других модулей
target_link_libraries(Attack PRIVATE
        Utility
       Interface
)

# Настройка стандарта C++ (C++20 для std::ranges)
target_compile_features(Attack PRIVATE cxx_std_20)