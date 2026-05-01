#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "castle.h"
#include "castle_repository.h"
#include "castle_service.h"
#include "config_reader.h"
#include "utility/entity_type.h"
#include "health_component.h"
#include "level_component.h"
#include "economy_service.h"
#include "id_service.h"
#include <memory>

using namespace TowerDefence;

TEST_CASE("Castle - Создание и базовые методы") {
    // Подготовка конфигурации для типа замка
    nlohmann::json config;
    config["name"] = "test_castle";
    config["levels"] = {
        {
            {"health", 100.0},
            {"max_health", 100.0},
            {"regen", 1.0}
        },
        {
            {"health", 150.0},
            {"max_health", 150.0},
            {"regen", 2.0}
        }
    };
    
    JsonReader configReader{config};
    
    // Создание типа замка
    std::vector<std::unique_ptr<IConfigReader>> castleConfigs;
    castleConfigs.push_back(std::make_unique<JsonReader>(nlohmann::json{}));
    for (const auto& level : config["levels"]) {
        castleConfigs.push_back(std::make_unique<JsonReader>(level));
    }
    
    UpgradableType castleType("test_castle", std::move(castleConfigs));
    
    SECTION("Создание замка") {
        auto regen = std::make_unique<BasicRegenable>(1.0f);
        Castle castle(castleType, 2, 100.0f, 100.0f, std::move(regen), 1, {5, 5});
        
        REQUIRE(castle.getID() == 1);
        REQUIRE(castle.getPosition().getX() == 5);
        REQUIRE(castle.getPosition().getY() == 5);
        REQUIRE(castle.getHealth() == 100.0f);
        REQUIRE(castle.getMaxHealth() == 100.0f);
        REQUIRE(castle.isAlive() == true);
        REQUIRE(castle.getLevel() == 1);
        REQUIRE(castle.getUpgradableType().getName() == "test_castle");
    }
    
    SECTION("Урон и лечение") {
        auto regen = std::make_unique<BasicRegenable>(1.0f);
        Castle castle(castleType, 2, 100.0f, 100.0f, std::move(regen), 1, {5, 5});
        
        // Нанесение урона
        castle.dealDamage(30.0f);
        REQUIRE(castle.getHealth() == 70.0f);
        REQUIRE(castle.isAlive() == true);
        
        // Лечение
        castle.dealHeal(20.0f);
        REQUIRE(castle.getHealth() == 90.0f);
        
        // Убийство
        castle.dealDamage(90.0f);
        REQUIRE(castle.getHealth() == 0.0f);
        REQUIRE(castle.isAlive() == false);
        

    }
    
    SECTION("Модификаторы здоровья") {
        auto regen = std::make_unique<BasicRegenable>(1.0f);
        Castle castle(castleType, 2, 100.0f, 100.0f, std::move(regen), 1, {5, 5});
        
        castle.multiplyMaxHealth(1.5f);
        REQUIRE(castle.getMaxHealth() == 150.0f);
        REQUIRE(castle.getHealth() == 100.0f);
        
        castle.multiplyAbsorbedDamage(0.5f);
        castle.dealDamage(100.0f);
        REQUIRE(castle.getHealth() == 50.0f);
    }
    
    SECTION("Улучшение замка") {
        auto regen = std::make_unique<BasicRegenable>(1.0f);
        Castle castle(castleType, 2, 100.0f, 100.0f, std::move(regen), 1, {5, 5});
        
        REQUIRE(castle.getLevel() == 1);
        castle.upgrade();
        REQUIRE(castle.getLevel() == 2);
        
        // После улучшения здоровье должно стать как в конфиге уровня 2
        REQUIRE(castle.getHealth() == 150.0f);
        REQUIRE(castle.getMaxHealth() == 150.0f);
    }
    
    SECTION("Ремонт замка") {
        auto regen = std::make_unique<BasicRegenable>(1.0f);
        Castle castle(castleType, 2, 100.0f, 100.0f, std::move(regen), 1, {5, 5});
        
        castle.dealDamage(60.0f);
        REQUIRE(castle.getHealth() == 40.0f);
        
        castle.repair(0.5f); // Восстанавливаем 50%
        REQUIRE(castle.getHealth() == 90.0f); // 40 + 100*0.5 = 90
    }
}

TEST_CASE("Castle - Конфигурация") {
    nlohmann::json config;
    config["name"] = "test_castle";
    config["levels"] = {
        {
            {"health", 100.0},
            {"max_health", 100.0},
            {"regen", 1.0}
        }
    };
    
    JsonReader configReader{config};
    
    std::vector<std::unique_ptr<IConfigReader>> castleConfigs;
    castleConfigs.push_back(std::make_unique<JsonReader>(nlohmann::json{}));
    castleConfigs.push_back(std::make_unique<JsonReader>(config["levels"][0]));
    
    UpgradableType castleType("test_castle", std::move(castleConfigs));
    
    SECTION("Сохранение в конфиг") {
        auto regen = std::make_unique<BasicRegenable>(1.0f);
        Castle castle(castleType, 1, 75.0f, 100.0f, std::move(regen), 42, {10, 20});
        
        nlohmann::json saveConfig;
        JsonReader writer{saveConfig};
        castle.toConfig(writer);
        
        REQUIRE(writer.getSize_t("id") == 42);
        REQUIRE(writer.getSize_t("level") == 1);
        REQUIRE(writer.getSize_t("x") == 10);
        REQUIRE(writer.getSize_t("y") == 20);
        REQUIRE(writer.getFloat("health") == 75.0f);
        REQUIRE(writer.getFloat("max_health") == 100.0f);
    }
    
    SECTION("Загрузка из конфига") {
        auto regen = std::make_unique<BasicRegenable>(1.0f);
        Castle castle(castleType, 1, 100.0f, 100.0f, std::move(regen), 1, {0, 0});
        
        nlohmann::json loadConfig;
        loadConfig["id"] = 99;
        loadConfig["level"] = 1;
        loadConfig["x"] = 15;
        loadConfig["y"] = 25;
        loadConfig["health"] = 80.0;
        loadConfig["max_health"] = 120.0;
        
        JsonReader loader{loadConfig};
        castle.setFromConfig(loader);
        
        REQUIRE(castle.getID() == 99);
        REQUIRE(castle.getPosition().getX() == 15);
        REQUIRE(castle.getPosition().getY() == 25);
        REQUIRE(castle.getHealth() == 80.0f);
        REQUIRE(castle.getMaxHealth() == 120.0f);
    }
}

TEST_CASE("CastleRepository - Управление замком") {
    nlohmann::json config;
    config["name"] = "basic_castle";
    config["levels"] = {
        {
            {"health", 100.0},
            {"max_health", 100.0},
            {"regen", 1.0}
        },
        {
            {"health", 150.0},
            {"max_health", 150.0},
            {"regen", 2.0}
        }
    };
    
    JsonReader configReader{config};
    
    SECTION("Создание репозитория") {
        CastleRepository repo(configReader);
        
        // Проверяем, что тип создан правильно
        const auto& type = repo.getTypeRef("basic_castle");
        REQUIRE(type.getName() == "basic_castle");
        REQUIRE(type.getMaxLevel() == 2);
    }
    

}

TEST_CASE("CastleService - Основные операции") {
    // Подготовка конфигурации
    nlohmann::json castleConfig;
    castleConfig["name"] = "basic_castle";
    castleConfig["levels"] = {
        {
            {"health", 100.0},
            {"max_health", 100.0},
            {"regen", 1.0}
        },
        {
            {"health", 150.0},
            {"max_health", 150.0},
            {"regen", 2.0}
        }
    };
    
    nlohmann::json economyConfig = {
        {"money", 1000},
        {"rewards", {{"test", 10}}},
        {"upgrades", {
            {"basic_castle", {
                {"max_level", 2},
                {"costs", {
                    {"1", 200},
                    {"2", 300}
                }}
            }}
        }}
    };
    
    JsonReader castleConfigReader{castleConfig};
    JsonReader economyConfigReader{economyConfig};
    
    SECTION("Создание сервиса") {
        CastleRepository repo(castleConfigReader);
        EconomyService economy(economyConfigReader);
        IDService idService;
        
        CastleService service(repo, idService, economy, {10, 10});
        
        REQUIRE(service.getPosition().getX() == 10);
        REQUIRE(service.getPosition().getY() == 10);
        REQUIRE(service.isAlive() == true);
        REQUIRE(service.getCastleID() == 1);
    }
    
    SECTION("Нанесение урона замку") {
        CastleRepository repo(castleConfigReader);
        EconomyService economy(economyConfigReader);
        IDService idService;
        
        CastleService service(repo, idService, economy, {10, 10});
        
        REQUIRE(service.isAlive() == true);
        service.dealDamage(50.0f);
        
        auto& castle = repo.getCastle();
        REQUIRE(castle.getHealth() == 50.0f);
        REQUIRE(service.isAlive() == true);
        
        service.dealDamage(60.0f);
        REQUIRE(castle.getHealth() == 0.0f);
        REQUIRE(service.isAlive() == false);
    }
    
    SECTION("Обновление состояния") {
        CastleRepository repo(castleConfigReader);
        EconomyService economy(economyConfigReader);
        IDService idService;
        
        CastleService service(repo, idService, economy, {10, 10});
        
        auto& castle = repo.getCastle();
        castle.dealDamage(20.0f);
        float initialHealth = castle.getHealth();
        
        service.update(1.0f); // Прошла 1 секунда, регенерация 1.0 в секунду
        
        REQUIRE(castle.getHealth() == initialHealth + 1.0f);
    }
}

TEST_CASE("CastleService - Улучшение и ремонт") {
    nlohmann::json castleConfig;
    castleConfig["name"] = "basic_castle";
    castleConfig["levels"] = {
        {
            {"health", 100.0},
            {"max_health", 100.0},
            {"regen", 1.0}
        },
        {
            {"health", 150.0},
            {"max_health", 150.0},
            {"regen", 2.0}
        }
    };
    
    nlohmann::json economyConfig = {
        {"money", 1000},
        {"rewards", {{"test", 10}}},
        {"upgrades", {
            {"basic_castle", {
                {"max_level", 2},
                {"costs", {
                    {"1", 200},
                    {"2", 300}
                }}
            }}
        }}
    };
    
    JsonReader castleConfigReader{castleConfig};
    JsonReader economyConfigReader{economyConfig};
    
    SECTION("Успешное улучшение") {
        CastleRepository repo(castleConfigReader);
        EconomyService economy(economyConfigReader);
        IDService idService;
        
        CastleService service(repo, idService, economy, {10, 10});
        
        REQUIRE(economy.balance() == 1000.0f);
        bool upgraded = service.upgradeCastle();
        
        REQUIRE(upgraded == true);
        REQUIRE(economy.balance() == 700.0f); // 1000 - 300
        
        auto& castle = repo.getCastle();
        REQUIRE(castle.getLevel() == 2);
        REQUIRE(castle.getMaxHealth() == 150.0f);
    }
    
    SECTION("Недостаточно денег для улучшения") {
        nlohmann::json poorEconomyConfig = {
            {"money", 250},
            {"rewards", {{"test", 10}}},
            {"upgrades", economyConfig["upgrades"]}
        };
        
        JsonReader poorEconomyReader{poorEconomyConfig};
        
        CastleRepository repo(castleConfigReader);
        EconomyService economy(poorEconomyReader);
        IDService idService;
        
        CastleService service(repo, idService, economy, {10, 10});
        
        REQUIRE(economy.balance() == 250.0f);
        bool upgraded = service.upgradeCastle();
        
        REQUIRE(upgraded == false);
        REQUIRE(economy.balance() == 250.0f);
        
        auto& castle = repo.getCastle();
        REQUIRE(castle.getLevel() == 1);
    }
    
    SECTION("Ремонт замка") {
        CastleRepository repo(castleConfigReader);
        EconomyService economy(economyConfigReader);
        IDService idService;
        
        CastleService service(repo, idService, economy, {10, 10});
        
        // Наносим урон
        service.dealDamage(60.0f);
        auto& castle = repo.getCastle();
        REQUIRE(castle.getHealth() == 40.0f);
        
        // Пытаемся починить (должно стоить 60% от стоимости улучшения уровня 920)
        bool repaired = service.repairCastle();
        
        REQUIRE(repaired == true);
        REQUIRE(economy.balance() == 920.0f);
        REQUIRE(castle.getHealth() == 100.0f); // Полностью отремонтирован
    }
}

TEST_CASE("CastleService - Сброс и конфигурация") {
    nlohmann::json castleConfig;
    castleConfig["name"] = "basic_castle";
    castleConfig["levels"] = {
        {
            {"health", 100.0},
            {"max_health", 100.0},
            {"regen", 1.0}
        }
    };
    
    nlohmann::json economyConfig = {
        {"money", 1000},
        {"rewards", {{"test", 10}}},
        {"upgrades", {
            {"basic_castle", {
                {"max_level", 1},
                {"costs", {{"1", 200}}}
            }}
        }}
    };
    
    JsonReader castleConfigReader{castleConfig};
    JsonReader economyConfigReader{economyConfig};


    
    SECTION("Сохранение и загрузка конфигурации") {
        CastleRepository repo(castleConfigReader);
        EconomyService economy(economyConfigReader);
        IDService idService;
        
        CastleService service(repo, idService, economy, {10, 10});
        
        // Наносим урон и меняем состояние
        service.dealDamage(30.0f);
        
        // Сохраняем
        nlohmann::json saveConfig;
        JsonReader writer{saveConfig};
        service.toConfig(writer);
        
        REQUIRE(writer.getFloat("health") == 70.0f);
        REQUIRE(writer.getSize_t("x") == 10);
        REQUIRE(writer.getSize_t("y") == 10);
        
        // Загружаем новое состояние
        nlohmann::json loadConfig;
        loadConfig["id"] = 2;
        loadConfig["level"] = 1;
        loadConfig["x"] = 20;
        loadConfig["y"] = 30;
        loadConfig["health"] = 80.0;
        loadConfig["max_health"] = 100.0;
        
        JsonReader loader{loadConfig};
        service.load(loader);
        
        auto& castle = repo.getCastle();
        REQUIRE(castle.getID() == 2);
        REQUIRE(castle.getPosition().getX() == 20);
        REQUIRE(castle.getPosition().getY() == 30);
        REQUIRE(castle.getHealth() == 80.0f);
    }
}