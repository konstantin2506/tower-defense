# Библиотека для модуля castle
add_library(Castle
        # Добавьте сюда castle файлы когда они появятся
        src/castle_service.cpp
        src/castle.cpp
        src/castle_repository.cpp
        # src/castle.cpp
        # src/castle_factory.cpp
        # src/castle_component.cpp
)

# Публичные include директории
target_include_directories(Castle PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Приватные include директории
target_include_directories(Castle PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Зависимости от других модулей
target_link_libraries(Castle PRIVATE
        Utility
        Interface
        Components
)

# Настройка стандарта C++
target_compile_features(Castle PRIVATE cxx_std_20)

