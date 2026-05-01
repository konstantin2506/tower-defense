#pragma once
#include <chrono>
#include <cstddef>
#include <unordered_map>

namespace TowerDefence
{
    using ID = std::size_t; ///< Тип для уникальных идентификаторов.
    using HashedName = std::size_t; ///< Тип для хешированных имен.
    using Level = std::size_t; ///< Тип для уровней объектов.
    using Money = std::size_t; ///< Тип для денежных значений.
    using duration_t = std::chrono::milliseconds; ///< Тип для временных интервалов.
    using time_point_t = std::chrono::steady_clock::time_point; ///< Тип для точек времени.

    /**
     * @brief Типы движения объектов.
     */
    enum class MoveType {
        LANDED, ///< Наземное движение.
        AIR, ///< Воздушное движение.
    };

    /**
     * @brief Типы тайлов на карте.
     */
    enum class TileType : int {
        WALL, ///< Стена.
        TOWER, ///< Башня.
        CASTLE, ///< Замок.
        LAND, ///< Земля.
        MOUNTAIN, ///< Гора.
        WATER ///< Вода.
    };

    /// Мапа для преобразования строк в TileType.
    static const std::unordered_map<std::string, TileType> tileTypeMap = {
        {"wall", TileType::WALL},
        {"tower", TileType::TOWER},
        {"castle", TileType::CASTLE},
        {"land", TileType::LAND},
        {"mountain", TileType::MOUNTAIN},
        {"water", TileType::WATER}
    };

    /// Мапа для преобразования TileType в строки.
    static const std::unordered_map<TileType, std::string> typeTileMap = {
        {TileType::LAND, "land"},
        {TileType::MOUNTAIN, "mountain"},
        {TileType::WATER, "water"},
        {TileType::CASTLE, "castle"},
    };

    /**
     * @brief Типы отрисовки объектов.
     */
    enum class DrawType{
        BACKGROUND, ///< Фоновые объекты.
        STATIC, ///< Статические объекты.
        DYNAMIC ///< Динамические объекты.
     };

    /// Мапа для преобразования строк в DrawType.
    static const std::unordered_map<std::string, DrawType> drawTypeMap = {
        {"back_ground", DrawType::BACKGROUND},
        {"static", DrawType::STATIC},
        {"dynamic", DrawType::DYNAMIC}
    };

    /// Мапа для преобразования числовых индексов в имена зданий.
    /// Нечетные индексы соответствуют башням, четные - стенам.
    static const std::unordered_map<size_t, std::string> buildings{
        {1, "basic_tower"},
        {2, "basic_wall"}
    };
}