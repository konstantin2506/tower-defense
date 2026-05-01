# Библиотека для модуля wall
add_library(WorldMap
    src/world_map.cpp
)

# Публичные include директории
target_include_directories(WorldMap PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Приватные include директории
target_include_directories(WorldMap PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Зависимости от других модулей
target_link_libraries(WorldMap PRIVATE
        Utility
        Interface
)

# Настройка стандарта C++
target_compile_features(WorldMap PRIVATE cxx_std_17)

