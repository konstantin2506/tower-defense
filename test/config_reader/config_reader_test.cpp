#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "config_reader.h"
#include <fstream>

using namespace TowerDefence;

TEST_CASE("JsonReader - Базовые операции")
{
    SECTION("Чтение простых типов")
    {
        json config = {
            {"test_string", "hello"},
            {"test_int", 123},
            {"test_float", 45.6f},
            {"test_bool", true},
            {"test_size_t", 789}
        };
        
        JsonReader reader{config};
        
        REQUIRE(reader.getString("test_string") == "hello");
        REQUIRE(reader.getInt("test_int") == 123);
        REQUIRE(reader.getBool("test_bool") == true);
        REQUIRE(reader.getSize_t("test_size_t") == 789);
    }
    
    SECTION("Запись простых типов")
    {
        json config;
        JsonReader writer{config};
        
        writer.writeString("field1", "value1");
        writer.writeInt("field2", 222);
        writer.writeFloat("field3", 33.3f);
        writer.writeBool("field4", false);
        writer.writeSize_t("field5", 444);
        
        REQUIRE(writer.getString("field1") == "value1");
        REQUIRE(writer.getInt("field2") == 222);
        REQUIRE(writer.getBool("field4") == false);
        REQUIRE(writer.getSize_t("field5") == 444);
    }
}

TEST_CASE("JsonReader - Файловые операции")
{
    SECTION("fromFile() загрузка из файла")
    {
        std::ofstream file("test_from_file.json");
        file << R"({
            "game_name": "TowerDefence",
            "version": 2,
            "enabled": true
        })";
        file.close();

        json config;
        JsonReader base{config};
        auto reader = base.fromFile("test_from_file.json");
        
        REQUIRE(reader->getString("game_name") == "TowerDefence");
        REQUIRE(reader->getInt("version") == 2);
        REQUIRE(reader->getBool("enabled") == true);
        
        std::remove("test_from_file.json");
    }
}

TEST_CASE("JsonReader - Вложенные структуры")
{
    SECTION("getNested()")
    {
        json config = {
            {"player", {
                {"name", "hero"},
                {"level", 10},
                {"health", 100.0f}
            }}
        };
        
        JsonReader reader{config};
        auto player = reader.getNested("player");
        
        REQUIRE(player->getString("name") == "hero");
        REQUIRE(player->getInt("level") == 10);
    }
    
    SECTION("Запись вложенных полей")
    {
        json config;
        JsonReader writer{config};
        
        writer.writeStringAt("stats", "name", "warrior");
        writer.writeIntAt("stats", "attack", 50);
        writer.writeFloatAt("stats", "speed", 1.5f);
        
        auto stats = writer.getNested("stats");
        REQUIRE(stats->getString("name") == "warrior");
        REQUIRE(stats->getInt("attack") == 50);
    }
}

TEST_CASE("JsonReader - Коллекции")
{
    SECTION("getItems()")
    {
        json config = {
            {"towers", {
                {"archer", {{"damage", 10}, {"range", 5}}},
                {"mage", {{"damage", 30}, {"range", 3}}}
            }}
        };
        
        JsonReader reader{config};
        auto towers = reader.getItems("towers");
        
        REQUIRE(towers.size() == 2);
        REQUIRE(towers["archer"]->getInt("damage") == 10);
        REQUIRE(towers["mage"]->getInt("damage") == 30);
    }
    
    SECTION("getStrings()")
    {
        json config = {
            {"messages", {
                {"welcome", "Hello player"},
                {"game_over", "You lost"},
                {"victory", "You won!"}
            }}
        };
        
        JsonReader reader{config};
        auto messages = reader.getStrings("messages");
        
        REQUIRE(messages.size() == 3);
        REQUIRE(messages["welcome"] == "Hello player");
        REQUIRE(messages["victory"] == "You won!");
    }
}

TEST_CASE("JsonReader - Ошибки")
{
    SECTION("Поле не найдено")
    {
        json config;
        JsonReader reader{config};
        
        REQUIRE_THROWS(reader.getString("missing_field"));
        REQUIRE_THROWS(reader.getInt("missing_field"));
        REQUIRE_THROWS(reader.getNested("missing_field"));
    }
    
    SECTION("Неправильный тип")
    {
        json config = {{"field", "not_a_number"}};
        JsonReader reader{config};
        
        REQUIRE_THROWS(reader.getInt("field"));
        REQUIRE_THROWS(reader.getFloat("field"));
        REQUIRE_THROWS(reader.getBool("field"));
    }
}