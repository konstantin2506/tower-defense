# Библиотека для модуля effects
add_library(Effects
        # Добавьте сюда effects файлы когда они появятся
        # src/effect_service.cpp
        src/effects.cpp
        # src/effect_applier.cpp
        # src/poison_effect.cpp
        # src/slow_effect.cpp
)

# Публичные include директории
target_include_directories(Effects PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Приватные include директории
target_include_directories(Effects PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Зависимости от других модулей
target_link_libraries(Effects PRIVATE
        Utility
        Interface
)
# Настройка стандарта C++
target_compile_features(Effects PRIVATE cxx_std_20)

