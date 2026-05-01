#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "economy_service.h"
#include "config_reader.h"
#include "utility/entity_type.h"

using namespace TowerDefence;

TEST_CASE("EconomyService - Пример конфигурации")
{
    json config = {
        {"money", 1000000},
        {"rewards", {
            {"archer", 50},
            {"balloon", 30},
            {"archer_boss", 500},
            {"warrior", 20},
            {"basic_wall", 100},
            {"basic_tower", 300}
        }},
        {"upgrades", {
            {"basic_tower", {
                {"max_level", 3},
                {"costs", {
                    {"1", 200},
                    {"2", 300},
                    {"3", 400}
                }}
            }},
            {"basic_wall", {
                {"max_level", 3},
                {"costs", {
                    {"1", 100},
                    {"2", 200},
                    {"3", 300}
                }}
            }}
        }}
    };

    JsonReader configReader{config};

    SECTION("Создание сервиса с примером конфига")
    {
        EconomyService service(configReader);

        REQUIRE(service.balance() == 1000000);
    }

    SECTION("Награды за убийства врагов")
    {
        EconomyService service(configReader);

        REQUIRE(service.balance() == 1000000);

        service.rewardForKill("archer");
        REQUIRE(service.balance() == 1000050);

        service.rewardForKill("balloon");
        REQUIRE(service.balance() == 1000080);

        service.rewardForKill("archer_boss");
        REQUIRE(service.balance() == 1000580);

        service.rewardForKill("warrior");
        REQUIRE(service.balance() == 1000600);
    }

    SECTION("Награды за разрушение зданий")
    {
        EconomyService service(configReader);

        service.rewardForKill("basic_wall");
        REQUIRE(service.balance() == 1000100);

        service.rewardForKill("basic_tower");
        REQUIRE(service.balance() == 1000400);
    }
}

TEST_CASE("EconomyService - Улучшение basic_tower")
{
    json config = {
        {"money", 1000},
        {"rewards", {{"test", 10}}},
        {"upgrades", {
            {"basic_tower", {
                {"max_level", 3},
                {"costs", {
                    {"1", 200},
                    {"2", 300},
                    {"3", 400}
                }}
            }}
        }}
    };

    JsonReader configReader{config};
    EconomyService service(configReader);

    SECTION("Проверка стоимости улучшений")
    {
        REQUIRE(service.canUpgrade("basic_tower", 1) == true);
        REQUIRE(service.canUpgrade("basic_tower", 2) == true);
        REQUIRE(service.canUpgrade("basic_tower", 3) == true);
        REQUIRE(service.canUpgrade("basic_tower", 4) == false); // Несуществующий уровень
    }

    SECTION("Улучшение башни")
    {
        std::vector<std::unique_ptr<IConfigReader>> towerConfigs;
        towerConfigs.push_back(nullptr);
        towerConfigs.push_back(nullptr);
        towerConfigs.push_back(nullptr);
        towerConfigs.push_back(nullptr);
        UpgradableType towerType("basic_tower", std::move(towerConfigs));

        REQUIRE(service.balance() == 1000);

        // Улучшение на уровень 1 (строительство)
        REQUIRE(service.tryBuild(towerType) == true);
        REQUIRE(service.balance() == 800);

        // Улучшение на уровень 2
        REQUIRE(service.tryUpgrade(towerType, 2) == true);
        REQUIRE(service.balance() == 500);

        // Улучшение на уровень 3
        REQUIRE(service.tryUpgrade(towerType, 3) == true);
        REQUIRE(service.balance() == 100);
    }

    SECTION("Недостаточно денег для улучшения")
    {
        std::vector<std::unique_ptr<IConfigReader>> towerConfigs;
        towerConfigs.push_back(nullptr);
        towerConfigs.push_back(nullptr);
        towerConfigs.push_back(nullptr);
        towerConfigs.push_back(nullptr);
        UpgradableType towerType("basic_tower", std::move(towerConfigs));

        json poorConfig = {
            {"money", 250},
            {"rewards", {{"test", 10}}},
            {"upgrades", config["upgrades"]}
        };
        JsonReader poorReader{poorConfig};
        EconomyService poorService(poorReader);

        REQUIRE(poorService.tryBuild(towerType) == true);
        REQUIRE(poorService.balance() == 50);

        REQUIRE(poorService.tryUpgrade(towerType, 2) == false);
        REQUIRE(poorService.balance() == 50);
    }
}

TEST_CASE("EconomyService - Улучшение basic_wall")
{
    json config = {
        {"money", 800},
        {"rewards", {{"test", 10}}},
        {"upgrades", {
            {"basic_wall", {
                {"max_level", 3},
                {"costs", {
                    {"1", 100},
                    {"2", 200},
                    {"3", 300}
                }}
            }}
        }}
    };

    JsonReader configReader{config};
    EconomyService service(configReader);

    SECTION("Стоимость улучшений стен")
    {
        REQUIRE(service.canUpgrade("basic_wall", 1) == true);
        REQUIRE(service.canUpgrade("basic_wall", 2) == true);
        REQUIRE(service.canUpgrade("basic_wall", 3) == true);
    }

    SECTION("Ремонт стен")
    {
        std::vector<std::unique_ptr<IConfigReader>> wallConfigs;
        wallConfigs.push_back(nullptr);
        wallConfigs.push_back(nullptr);
        wallConfigs.push_back(nullptr);
        wallConfigs.push_back(nullptr);
        UpgradableType wallType("basic_wall", std::move(wallConfigs));

        // Полный ремонт уровня 1 (полностью разрушена)
        REQUIRE(service.tryRepair(wallType, 1, 0.0f) == true);
        REQUIRE(service.balance() == 800);

        service.reset();

        // Частичный ремонт уровня 2 (повреждена на 50%)
        REQUIRE(service.tryRepair(wallType, 2, 0.5f) == true);
        REQUIRE(service.balance() == 700); // 800 - 100 (50% от 200)
    }
}

TEST_CASE("EconomyService - Оплата разрушения")
{
    json config = {
        {"money", 500},
        {"rewards", {
            {"basic_tower", 300},
            {"basic_wall", 100}
        }},
        {"upgrades", {}}
    };

    JsonReader configReader{config};
    EconomyService service(configReader);

    SECTION("Оплата разрушения башни")
    {
        std::vector<std::unique_ptr<IConfigReader>> towerConfigs;
        towerConfigs.push_back(nullptr);
        UpgradableType towerType("basic_tower", std::move(towerConfigs));

        REQUIRE(service.payDestruction(towerType) == true);
        REQUIRE(service.balance() == 200);
    }

    SECTION("Оплата разрушения стены")
    {
        std::vector<std::unique_ptr<IConfigReader>> wallConfigs;
        wallConfigs.push_back(nullptr);
        UpgradableType wallType("basic_wall", std::move(wallConfigs));

        REQUIRE(service.payDestruction(wallType) == true);
        REQUIRE(service.balance() == 400);
    }

    SECTION("Недостаточно денег для оплаты разрушения")
    {
        std::vector<std::unique_ptr<IConfigReader>> towerConfigs;
        towerConfigs.push_back(nullptr);
        UpgradableType towerType("basic_tower", std::move(towerConfigs));

        json poorConfig = {{"money", 250}, {"rewards", config["rewards"]}, {"upgrades", {}}};
        JsonReader poorReader{poorConfig};
        EconomyService poorService(poorReader);

        REQUIRE(poorService.payDestruction(towerType) == false);
        REQUIRE(poorService.balance() == 250);
    }
}

TEST_CASE("EconomyService - Сброс состояния")
{
    json config = {
        {"money", 1000000},
        {"rewards", {
                {"archer", 50},
                {"basic_tower", 300}
        }},
        {"upgrades", {
                {"basic_tower", {
                    {"max_level", 3},
                    {"costs", {
                        {"1", 200},
                        {"2", 300},
                        {"3", 400}
                    }}
                }}
        }}
    };

    JsonReader configReader{config};
    EconomyService service(configReader);

    SECTION("reset возвращает начальный баланс")
    {
        std::vector<std::unique_ptr<IConfigReader>> configs;
        configs.push_back(nullptr);
        configs.push_back(nullptr);
        UpgradableType towerType("basic_tower", std::move(configs));

        // Тратим деньги
        service.tryBuild(towerType);
        service.rewardForKill("archer");
        REQUIRE(service.balance() == 1000000 - 200 + 50);

        // Сбрасываем
        service.reset();
        REQUIRE(service.balance() == 1000000);
    }
}

TEST_CASE("EconomyService - Конфигурация")
{
    json config = {
        {"money", 1000000},
        {"rewards", {{"archer", 50}}},
        {"upgrades", {
            {"basic_tower", {
                {"max_level", 1},
                {"costs", {{"1", 200}}}
            }}
        }}
    };

    JsonReader configReader{config};
    EconomyService service(configReader);

    SECTION("Сохранение баланса")
    {
        service.rewardForKill("archer");
        REQUIRE(service.balance() == 1000050);

        json saveConfig;
        JsonReader writer{saveConfig};
        service.toConfig(writer);

        REQUIRE(writer.getSize_t("money") == 1000050);
    }

    SECTION("Загрузка баланса")
    {
        json loadConfig = {{"money", 750000}};
        JsonReader loader{loadConfig};
        service.setFromConfig(loader);

        REQUIRE(service.balance() == 750000);
    }
}

TEST_CASE("EconomyService - Комплексный тест")
{
    json config = {
        {"money", 1500},
        {"rewards", {
            {"archer", 50},
            {"basic_tower", 300}
        }},
        {"upgrades", {
            {"basic_tower", {
                {"max_level", 3},
                {"costs", {
                    {"1", 200},
                    {"2", 300},
                    {"3", 400}
                }}
            }}
        }}
    };

    JsonReader configReader{config};
    EconomyService service(configReader);

    std::vector<std::unique_ptr<IConfigReader>> towerConfigs;
    towerConfigs.push_back(nullptr);
    towerConfigs.push_back(nullptr);
    towerConfigs.push_back(nullptr);
    towerConfigs.push_back(nullptr);
    UpgradableType towerType("basic_tower", std::move(towerConfigs));

    SECTION("Полный цикл: стройка -> улучшения -> награды -> разрушение")
    {
        REQUIRE(service.balance() == 1500);

        // Строим башню
        REQUIRE(service.tryBuild(towerType) == true);
        REQUIRE(service.balance() == 1300);

        // Убиваем врагов
        service.rewardForKill("archer");
        service.rewardForKill("archer");
        service.rewardForKill("archer");
        REQUIRE(service.balance() == 1450);

        // Улучшаем до уровня 2
        REQUIRE(service.tryUpgrade(towerType, 2) == true);
        REQUIRE(service.balance() == 1150);

        // Еще награды
        service.rewardForKill("archer");
        REQUIRE(service.balance() == 1200);

        // Улучшаем до уровня 3
        REQUIRE(service.tryUpgrade(towerType, 3) == true);
        REQUIRE(service.balance() == 800);

        // Оплачиваем разрушение
        REQUIRE(service.payDestruction(towerType) == true);
        REQUIRE(service.balance() == 500);

        // Сброс
        service.reset();
        REQUIRE(service.balance() == 1500);
    }
}