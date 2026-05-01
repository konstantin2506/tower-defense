# Библиотека для модуля enemy
add_library(Enemy
        # Добавьте сюда enemy файлы когда они появятся
        src/enemy_service.cpp
        src/enemy_factory.cpp
        src/enemy_repository.cpp
        src/enemy.cpp
        src/enemy_factory_manager.cpp
        # src/enemy.cpp
        # src/enemy_factory.cpp
)

# Публичные include директории
target_include_directories(Enemy PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Приватные include директории
target_include_directories(Enemy PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Зависимости от других модулей
target_link_libraries(Enemy PRIVATE
        Utility
        Interface
        Components
        Attack
        Ability
        Grid
        Effects
)

# Настройка стандарта C++
target_compile_features(Enemy PRIVATE cxx_std_20)