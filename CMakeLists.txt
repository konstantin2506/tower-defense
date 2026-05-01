cmake_minimum_required(VERSION 3.15)
project(TowerDefence)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()
find_package(Catch2 REQUIRED)

add_definitions(-DPARALLEL)
find_package(Threads REQUIRED)
#add_compile_options(-fsanitize=thread)
#add_link_options(-fsanitize=thread)
add_subdirectory(utility)
add_subdirectory(interface)
add_subdirectory(world_map)

add_subdirectory(id_service)
add_subdirectory(config_reader)
add_subdirectory(components)

add_subdirectory(economy)
add_subdirectory(pathfinding)
add_subdirectory(grid)

add_subdirectory(effects)
add_subdirectory(attack)
add_subdirectory(ability)

add_subdirectory(tower)
add_subdirectory(wall)
add_subdirectory(castle)
add_subdirectory(enemy)

add_subdirectory(view)
add_subdirectory(game_loader)

add_subdirectory(presenter)

add_executable(lab3 main.cpp)


target_link_libraries(lab3 PRIVATE
        Threads::Threads
        Utility
        Config
        Interface
        Economy
        Grid
        Pathfinding
        Components
        Effects
        IDService
        Towers
        Castle
        Wall
        Attack
        Ability
        Enemy
        Loader
        View
        Presenter
)


add_subdirectory(test)
