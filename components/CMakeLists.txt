# Библиотека для модуля components
add_library(Components
        src/health_component.cpp
        src/level_component.cpp
        src/ability_component.cpp
        src/move_component.cpp
        src/basic_regen.cpp
        # Добавьте другие компоненты когда они появятся
        # src/level_component.cpp
        # src/attack_component.cpp
        # src/movement_component.cpp
)

# Публичные include директории
target_include_directories(Components PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Приватные include директории
target_include_directories(Components PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Зависимости от других модулей
target_link_libraries(Components PRIVATE
        Utility
        Interface
)

# Настройка стандарта C++
target_compile_features(Components PRIVATE cxx_std_20)
