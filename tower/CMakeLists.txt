add_library(Towers
        src/tower_factory.cpp
        src/tower_factory_manager.cpp
        src/tower.cpp
        src/tower_repository.cpp
        src/tower_service.cpp
)

# Публичные include директории (будут доступны при линковке)
target_include_directories(Towers PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Приватные include директории (только для этой библиотеки)
target_include_directories(Towers PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Если есть зависимости от других модулей
target_link_libraries(Towers PRIVATE
        Utility
        Interface
        Attack
        Components
        Grid
    )

# Настройка стандарта C++
target_compile_features(Towers PRIVATE cxx_std_20)