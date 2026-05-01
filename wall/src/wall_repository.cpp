#include "wall_repository.h"

#include <ranges>

using namespace TowerDefence;

WallRepository::WallRepository(const IConfigReader &allWallConfigs)
{
    for (auto& [wallName, config] : allWallConfigs.getItems("walls")) {
        addType(wallName, *config);
    }
}

void WallRepository::addType(const std::string& name, const IConfigReader& wallConfig)
{
    std::vector<std::unique_ptr<IConfigReader>> wallLevels;
    wallLevels.push_back(nullptr);
    for (auto& config : wallConfig.getItems("levels") | std::views::values) {
        wallLevels.emplace_back(std::move(config));
    }
    wallTypes_.emplace(name, UpgradableType(name, std::move(wallLevels)));
}

const UpgradableType & WallRepository::getTypeRef(const std::string &name) const
{
    return wallTypes_.at(name);
}

void WallRepository::forEach(std::function<void(IWall &)> callback) const
{
    for (auto& wall : allWalls | std::views::values) {
        callback(*wall);
    }
}

void WallRepository::addWall(ID id, std::unique_ptr<IWall> wall)
{
    allWalls.emplace(id, std::move(wall));
}

void WallRepository::removeWall(ID id)
{
    allWalls.erase(id);
}
