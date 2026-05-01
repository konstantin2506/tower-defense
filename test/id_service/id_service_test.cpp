#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "id_service.h"

#include "../../config_reader/include/config_reader.h"

using namespace TowerDefence;

TEST_CASE("IDService - Базовые тесты")
{
    SECTION("getID() возвращает последовательные ID")
    {
        IDService service;

        auto id1 = service.getID();
        auto id2 = service.getID();
        auto id3 = service.getID();

        REQUIRE(id1 == 1);
        REQUIRE(id2 == 2);
        REQUIRE(id3 == 3);
        REQUIRE(id1 != id2);
        REQUIRE(id2 != id3);
    }
    SECTION("setFromConfig()")
    {
        IDService service;

        json config;
        JsonReader reader{config};
        auto cfg = reader.fromFile("../../../test/id_service/from.json");
        service.setFromConfig(*cfg);
        REQUIRE(service.getID() == 6);
    }
    SECTION("toConfig()")
    {
        json config;
        JsonReader reader{config};
        auto cfg = reader.fromFile("../../../test/id_service/to.json");
        IDService service;
        service.getID();
        service.toConfig(*cfg);
        REQUIRE(cfg->getSize_t("counter") == 2);

    }
}