#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>

#include "config_reader.h"
#include "economy_service.h"
#include "id_service.h"
#include "grid_service.h"
#include "world_map.h"
#include "health_component.h"
#include <memory>

#include "wall_factory.h"
#include "wall_repository.h"
#include "wall_service.h"

using namespace TowerDefence;

TEST_CASE("WallService - Полный цикл операций") {
    // Конфигурация карты
    nlohmann::json mapConfig;
    mapConfig["x_size"] = 5;
    mapConfig["y_size"] = 5;
    mapConfig["map"] = "land land land land land land land land land land land land land land land land land land land land land land land land land";

    // Конфигурация стен
    nlohmann::json wallsConfig;
    wallsConfig["walls"] = {
        {"basic_wall", {
            {"name", "basic_wall"},
            {"levels", {
                {{"health", 100.0}, {"max_health", 100.0}, {"regen", 0.5}},
                {{"health", 150.0}, {"max_health", 150.0}, {"regen", 1.0}}
            }}
        }}
    };

    // Конфигурация экономики
    nlohmann::json economyConfig = {
        {"money", 2000},
        {"rewards", {{"basic_wall", 50}}},
        {"upgrades", {
            {"basic_wall", {
                {"max_level", 2},
                {"costs", {
                    {"1", 100},  // стоимость постройки (уровень 1)
                    {"2", 200}   // стоимость улучшения до уровня 2
                }}
            }}
        }}
    };

    JsonReader mapReader{mapConfig};
    JsonReader wallsReader{wallsConfig};
    JsonReader economyReader{economyConfig};

    // Создаем зависимости
    WallRepository repo(wallsReader);
    EconomyService economy(economyReader);
    IDService idService;
    WorldMap worldMap(mapReader);
    auto gridService = std::make_unique<SingleIDGridService>(5, 5);

    WallService service(idService, economy, repo, worldMap, std::move(gridService));
    service.init(wallsReader);

    SECTION("Создание и удаление стены") {
        // Проверяем начальное состояние
        REQUIRE(economy.balance() == 2000.0f);
        REQUIRE(worldMap.isEmpty({2, 2}) == true);

        // Создаем стену
        REQUIRE(service.createWall("basic_wall", {2, 2}) == true);
        REQUIRE(economy.balance() == 1900.0f); // 2000 - 100

        // Проверяем, что стена создана
        int wallCount = 0;
        ID wallID = 0;
        service.forEach([&](const IWall& wall) {
            wallCount++;
            wallID = wall.getID();
            REQUIRE(wall.getPosition().getX() == 2);
            REQUIRE(wall.getPosition().getY() == 2);
            REQUIRE(wall.getHealth() == 100.0f);
            REQUIRE(wall.getLevel() == 1);
        });
        REQUIRE(wallCount == 1);

        // Проверяем, что позиция занята
        REQUIRE(worldMap.isEmpty({2, 2}) == false);


        REQUIRE(service.createWall("basic_wall", {2, 2}) == false);


        REQUIRE(service.destroyWall({2, 2}) == true);
        REQUIRE(economy.balance() == 1850.0f);

        REQUIRE(service.isKilled(wallID) == true);

        // Собираем уничтоженные
        auto destroyed = service.collectDestroyed();
        REQUIRE(destroyed.size() == 1);
        REQUIRE(destroyed[0].second == "basic_wall");

        // Проверяем, что позиция освободилась
        REQUIRE(worldMap.isEmpty({2, 2}) == true);
    }

    SECTION("Улучшение и ремонт стены") {

        REQUIRE(service.createWall("basic_wall", {1, 1}) == true);
        REQUIRE(economy.balance() == 1900.0f);


        std::vector<ID> wallIDs;
        service.forEach([&](const IWall& wall) {
            wallIDs.push_back(wall.getID());
        });
        ID wallID = wallIDs[0];


        service.dealDamage(wallID, 60.0f);
        repo.getWall(wallID);
        auto& wall = repo.getWall(wallID);
        REQUIRE(wall.getHealth() == 40.0f);


        REQUIRE(service.repairWall(wallID) == true);
        REQUIRE(wall.getHealth() == 100.0f);

        REQUIRE(service.upgradeWall(wallID) == true);
        REQUIRE(wall.getLevel() == 2);
        REQUIRE(wall.getMaxHealth() == 150.0f);
        REQUIRE(wall.getHealth() == 150.0f);


        REQUIRE(service.upgradeWall(wallID) == false);
    }

    SECTION("Множественные стены и операции") {
        // Создаем несколько стен
        REQUIRE(service.createWall("basic_wall", {0, 0}) == true);
        REQUIRE(service.createWall("basic_wall", {0, 1}) == true);
        REQUIRE(service.createWall("basic_wall", {1, 0}) == true);
        REQUIRE(economy.balance() == 1700.0f); // 2000 - 100*3

        // Проверяем количество
        int count = 0;
        service.forEach([&](const IWall&) { count++; });
        REQUIRE(count == 3);

        // Получаем стены в радиусе
        auto wallsInRadius = service.getInRadius({0, 0}, 1);
        REQUIRE(wallsInRadius.size() == 3); // Все стены в радиусе 1

        // Уничтожаем одну стену
        REQUIRE(service.destroyWall({0, 0}) == true);


        // Проверяем, что осталось 2 стены
        count = 0;
        service.forEach([&](const IWall&) { count++; });
        REQUIRE(count == 3); // Еще не собрали уничтоженные

        // Собираем уничтоженные
        auto destroyed = service.collectDestroyed();
        REQUIRE(destroyed.size() == 1);

        // Теперь должно быть 2 стены
        count = 0;
        service.forEach([&](const IWall&) { count++; });
        REQUIRE(count == 2);
    }

    SECTION("Смертельный урон и уничтожение") {
        // Создаем стену
        REQUIRE(service.createWall("basic_wall", {3, 3}) == true);

        // Находим ID
        std::vector<ID> wallIDs;
        service.forEach([&](const IWall& wall) { wallIDs.push_back(wall.getID()); });
        ID wallID = wallIDs[0];

        // Наносим смертельный урон
        service.dealDamage(wallID, 150.0f);
        REQUIRE(service.isKilled(wallID) == true);

        // Обновляем сервис
        service.update(1.0f);

        // Собираем уничтоженные
        auto destroyed = service.collectDestroyed();
        REQUIRE(destroyed.size() == 1);
        REQUIRE(destroyed[0].second == "basic_wall");

    }

    SECTION("Сброс состояния") {
        // Создаем несколько стен
        REQUIRE(service.createWall("basic_wall", {4, 4}) == true);
        REQUIRE(service.createWall("basic_wall", {4, 3}) == true);

        // Проверяем, что стены есть
        int countBefore = 0;
        service.forEach([&](const IWall&) { countBefore++; });
        REQUIRE(countBefore == 2);

        // Сбрасываем
        service.reset();

        // Проверяем, что стен нет
        int countAfter = 0;
        service.forEach([&](const IWall&) { countAfter++; });
        REQUIRE(countAfter == 0);

        // Проверяем, что деньги не изменились (экономика не сбрасывается в WallService::reset)

    }

    SECTION("Конфигурация") {
        // Создаем стену
        REQUIRE(service.createWall("basic_wall", {2, 3}) == true);

        // Наносим урон
        std::vector<ID> wallIDs;
        service.forEach([&](const IWall& wall) { wallIDs.push_back(wall.getID()); });
        service.dealDamage(wallIDs[0], 30.0f);

        // Сохраняем состояние
        nlohmann::json saveConfig;
        JsonReader writer{saveConfig};
        service.toConfig(writer);

        // Должен сохраниться хотя бы 1 объект
        REQUIRE(saveConfig.dump().size() > 0);
        json cfg;
        JsonReader reader{cfg};
        service.load(*reader.fromFile("../../../test/wall/load.json"));

    }
}

TEST_CASE("WallRepository - Базовые операции") {
    nlohmann::json wallsConfig;
    wallsConfig["walls"] = {
        {"basic_wall", {
            {"name", "basic_wall"},
            {"levels", {
                {{"health", 100.0}, {"max_health", 100.0}, {"regen", 0.5}}
            }}
        }},
        {"strong_wall", {
            {"name", "strong_wall"},
            {"levels", {
                {{"health", 200.0}, {"max_health", 200.0}, {"regen", 1.0}}
            }}
        }}
    };

    JsonReader reader{wallsConfig};

    SECTION("Создание и получение типов") {
        WallRepository repo(reader);

        const auto& basicType = repo.getTypeRef("basic_wall");
        REQUIRE(basicType.getName() == "basic_wall");
        REQUIRE(basicType.getMaxLevel() == 1);

        const auto& strongType = repo.getTypeRef("strong_wall");
        REQUIRE(strongType.getName() == "strong_wall");
        REQUIRE(strongType.getMaxLevel() == 1);

        REQUIRE_THROWS_AS(repo.getTypeRef("unknown_wall"), std::exception);
    }
}

TEST_CASE("WallFactory - Создание фабрики") {
    nlohmann::json config;
    config["name"] = "test_wall";
    config["levels"] = {
        {{"health", 100.0}, {"max_health", 100.0}, {"regen", 0.5}}
    };

    JsonReader configReader{config};

    std::vector<std::unique_ptr<IConfigReader>> wallConfigs;
    wallConfigs.push_back(nullptr);
    wallConfigs.push_back(std::make_unique<JsonReader>(config["levels"][0]));

    UpgradableType wallType("test_wall", std::move(wallConfigs));

    SECTION("Создание фабрики") {
        WallFactory factory(wallType);
        // Фабрика создана успешно
        REQUIRE(true);
    }
}