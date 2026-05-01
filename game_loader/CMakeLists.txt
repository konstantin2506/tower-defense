# Библиотека для модуля Loader
add_library(Loader
        src/game_loader.cpp
)

# Публичные include директории
target_include_directories(Loader PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Приватные include директории
target_include_directories(Loader PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Зависимости от других модулей
target_link_libraries(Loader PRIVATE
        Utility
        Interface
        Economy
        IDService
        WorldMap
        Pathfinding
        Enemy
        Wall
        Towers
        Castle
        Ability
        Attack
        Grid
        View
        Presenter
)

# Настройка стандарта C++
target_compile_features(Loader PRIVATE cxx_std_20)