add_library(View
    src/view.cpp
)

# Публичные include директории
target_include_directories(View PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Приватные include директории
target_include_directories(View PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(View PRIVATE Utility Interface sfml-graphics sfml-window sfml-system)

target_compile_features(View PRIVATE cxx_std_20)