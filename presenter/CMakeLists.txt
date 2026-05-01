add_library(Presenter
    src/presenter.cpp
)

# Публичные include директории
target_include_directories(Presenter PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Приватные include директории
target_include_directories(Presenter PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(Presenter PRIVATE Utility Interface View)

target_compile_features(Presenter PRIVATE cxx_std_20)