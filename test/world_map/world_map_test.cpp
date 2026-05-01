#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "world_map.h"

#include "config_reader.h"

using namespace TowerDefence;

TEST_CASE("WorldMap - Конструктор и инициализация")
{
    SECTION("Создание из конфига")
    {
        json config = {
            {"x_size", 3},
            {"y_size", 4},
            {"map", "land water land land land water water land land land land land"}
        };
        
        JsonReader reader{config};
        WorldMap map(reader);
        
        REQUIRE(map.size().getX() == 3);
        REQUIRE(map.size().getY() == 4);
        REQUIRE(map.getTile(0, 0) == TileType::LAND);
        REQUIRE(map.getTile(0, 1) == TileType::WATER);
        REQUIRE(map.getTile(2, 3) == TileType::LAND);
    }
    
    SECTION("Пустая карта")
    {
        json config = {
            {"x_size", 0},
            {"y_size", 0},
            {"map", ""}
        };
        
        JsonReader reader{config};
        WorldMap map(reader);
        
        REQUIRE(map.size().getX() == 0);
        REQUIRE(map.size().getY() == 0);
    }
}

TEST_CASE("WorldMap - Проверка пустоты")
{
    json config = {
        {"x_size", 3},
        {"y_size", 3},
        {"map", "land water land land land water land water land"}
    };
    
    JsonReader reader{config};
    WorldMap map(reader);
    
    SECTION("isEmpty() проверяет land")
    {
        REQUIRE(map.isEmpty({0, 0}) == true);    // land
        REQUIRE(map.isEmpty({0, 1}) == false);   // water
        REQUIRE(map.isEmpty({1, 1}) == true);    // land
    }
    
    SECTION("isEmptyWithCriteria() с разными критериями")
    {
        REQUIRE(map.isEmptyWithCriteria({0, 1}, TileType::WATER) == true);   // water == water
        REQUIRE(map.isEmptyWithCriteria({0, 0}, TileType::WATER) == false);  // land != water
        REQUIRE(map.isEmptyWithCriteria({1, 2}, TileType::WATER) == true);   // water == water
    }
}

TEST_CASE("WorldMap - Изменение тайлов")
{
    json config = {
        {"x_size", 2},
        {"y_size", 2},
        {"map", "land land land land"}
    };
    
    JsonReader reader{config};
    WorldMap map(reader);
    
    SECTION("setTileType меняет тип и устанавливает флаг обновления")
    {
        REQUIRE(map.getTile(0, 0) == TileType::LAND);
        REQUIRE(map.isUpdated() == true);
        
        map.updateHandled();
        REQUIRE(map.isUpdated() == false);
        
        map.setTileType({0, 0}, TileType::WATER);
        REQUIRE(map.getTile(0, 0) == TileType::WATER);
        REQUIRE(map.isUpdated() == true);
        
        map.setTileType({1, 1}, TileType::MOUNTAIN);
        REQUIRE(map.getTile(1, 1) == TileType::MOUNTAIN);
    }
    
    SECTION("Несколько изменений")
    {
        map.updateHandled();
        
        map.setTileType({0, 1}, TileType::TOWER);
        REQUIRE(map.getTile(0, 1) == TileType::TOWER);
        
        map.setTileType({1, 0}, TileType::WALL);
        REQUIRE(map.getTile(1, 0) == TileType::WALL);
        
        REQUIRE(map.isUpdated() == true);
    }
}

TEST_CASE("WorldMap - Состояние обновления")
{
    json config = {
        {"x_size", 1},
        {"y_size", 1},
        {"map", "land"}
    };
    
    JsonReader reader{config};
    WorldMap map(reader);
    
    SECTION("Флаг isUpdated по умолчанию true")
    {
        REQUIRE(map.isUpdated() == true);
    }
    
    SECTION("updateHandled сбрасывает флаг")
    {
        map.updateHandled();
        REQUIRE(map.isUpdated() == false);
    }
    
    SECTION("setTileType устанавливает флаг")
    {
        map.updateHandled();
        map.setTileType({0, 0}, TileType::WATER);
        REQUIRE(map.isUpdated() == true);
    }
    
    SECTION("reset устанавливает флаг")
    {
        map.updateHandled();
        REQUIRE(map.isUpdated() == false);
        
        map.reset();
        REQUIRE(map.isUpdated() == true);
    }
}

TEST_CASE("WorldMap - Граничные случаи")
{
    SECTION("Выход за границы при setTileType")
    {
        json config = {
            {"x_size", 2},
            {"y_size", 2},
            {"map", "land land land land"}
        };
        
        JsonReader reader{config};
        WorldMap map(reader);
        
        REQUIRE_NOTHROW(map.setTileType({0, 0}, TileType::WATER));
        REQUIRE_NOTHROW(map.setTileType({1, 1}, TileType::WATER));
        
        REQUIRE_THROWS(map.setTileType({2, 0}, TileType::WATER));
        REQUIRE_THROWS(map.setTileType({0, 2}, TileType::WATER));
    }
    
    SECTION("Выход за границы при проверке пустоты")
    {
        json config = {
            {"x_size", 1},
            {"y_size", 1},
            {"map", "land"}
        };
        
        JsonReader reader{config};
        WorldMap map(reader);
        
        REQUIRE_NOTHROW(map.isEmpty({0, 0}));
        REQUIRE_THROWS(map.isEmpty({1, 0}));
        REQUIRE_THROWS(map.isEmpty({0, 1}));
    }
}

TEST_CASE("WorldMap - Размеры и доступ")
{
    SECTION("Карта 5x3")
    {
        json config = {
            {"x_size", 5},
            {"y_size", 3},
            {"map", "land water land water land water land water land water land water land water land"}
        };
        
        JsonReader reader{config};
        WorldMap map(reader);
        
        REQUIRE(map.size().getX() == 5);
        REQUIRE(map.size().getY() == 3);
        
        REQUIRE(map.getTile(4, 2) == TileType::LAND);
        REQUIRE(map.getTile(2, 1) == TileType::WATER);
    }
    
    SECTION("Все типы тайлов")
    {
        json config = {
            {"x_size", 2},
            {"y_size", 3},
            {"map", "land water mountain tower wall castle"}
        };
        
        JsonReader reader{config};
        WorldMap map(reader);
        
        REQUIRE(map.getTile(0, 0) == TileType::LAND);
        REQUIRE(map.getTile(0, 1) == TileType::WATER);
        REQUIRE(map.getTile(0, 2) == TileType::MOUNTAIN);
        REQUIRE(map.getTile(1, 0) == TileType::TOWER);
        REQUIRE(map.getTile(1, 1) == TileType::WALL);
        REQUIRE(map.getTile(1, 2) == TileType::CASTLE);
    }
}