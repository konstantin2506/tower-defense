add_library(Pathfinding
    src/pathfinding.cpp
)

# Публичные include директории
target_include_directories(Pathfinding PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Приватные include директории
target_include_directories(Pathfinding PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(Pathfinding PRIVATE Utility Interface)

target_compile_features(Pathfinding PRIVATE cxx_std_20)