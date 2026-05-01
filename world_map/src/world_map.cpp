#include "world_map.h"

using namespace TowerDefence;

WorldMap::WorldMap(const IConfigReader &config)
{
    auto x = config.getSize_t("x_size");
    auto y = config.getSize_t("y_size");
    fullMap_ = Matrix<TileType>(x, y);

    auto mapString = config.getString("map");
    auto tiles = std::views::split(mapString, ' ')
                | std::views::transform([](auto&& range){
                    return std::string(range.begin(), range.end());
                });

    auto it = tiles.begin();
    for (auto& tile: fullMap_) {
        tile = tileTypeMap.at(*it);
        ++it;
    }
}

bool WorldMap::isEmpty(Vector2<size_t> point) const
{
    return isEmptyWithCriteria(point, TileType::LAND);
}

bool WorldMap::isEmptyWithCriteria(Vector2<size_t> point, TileType emptyCriteria) const
{
    return fullMap_.at(point.getX(), point.getY()) == emptyCriteria;
}

void WorldMap::setTileType(Vector2<size_t> point, TileType type)
{
    fullMap_.at(point.getX(), point.getY()) = type;
    isUpdated_= true;
}