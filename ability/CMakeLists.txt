# Библиотека для модуля ability
add_library(Ability
        # Добавьте сюда ability файлы когда они появятся
        # src/ability_service.cpp
        # src/ability.cpp
        src/dodge.cpp
        src/die_drop.cpp
        # src/fireball_ability.cpp
        # src/freeze_ability.cpp
        # src/heal_ability.cpp
)

# Публичные include директории
target_include_directories(Ability PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Приватные include директории
target_include_directories(Ability PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Зависимости от других модулей
target_link_libraries(Ability PRIVATE
        Utility
        Interface
        Effects
)

# Настройка стандарта C++
target_compile_features(Ability PRIVATE cxx_std_20)

