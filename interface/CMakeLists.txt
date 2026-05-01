add_library(Interface INTERFACE
)

# Публичные include директории
target_include_directories(Interface INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Interface INTERFACE Utility)

target_compile_features(Interface INTERFACE cxx_std_20)