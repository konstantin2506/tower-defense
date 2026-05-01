add_library(Grid
        src/singleID_grid_service.cpp
        src/vecID_grid_service.cpp
)

# Публичные include директории
target_include_directories(Grid PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Приватные include директории
target_include_directories(Grid PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(Grid PRIVATE Utility Interface)

# Настройка стандарта C++ (C++20 для std::ranges и std::views)
target_compile_features(Grid PRIVATE cxx_std_20)