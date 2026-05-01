# Библиотека для модуля wall
add_library(Wall
        src/wall_factory.cpp
        src/wall_factory_manager.cpp
        src/wall_service.cpp
        include/wall_repository.h
        src/wall_repository.cpp
        # Добавьте другие wall файлы когда они появятся
         src/wall.cpp
        # src/base_wall.cpp
        # src/upgradable_wall.cpp
)

# Публичные include директории
target_include_directories(Wall PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Приватные include директории
target_include_directories(Wall PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Зависимости от других модулей
target_link_libraries(Wall PRIVATE
        Utility
        Interface
        Config
        Economy
        Grid
        Components
        Effects
)

# Настройка стандарта C++
target_compile_features(Wall PRIVATE cxx_std_17)

