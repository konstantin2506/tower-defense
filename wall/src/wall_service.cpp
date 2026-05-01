#include "wall_service.h"

#include "grid_service.h"

using namespace TowerDefence;

void WallService::init(const IConfigReader &config)
{
    wallFactoryManager_.init(config, wallRepository_);
}

bool WallService::upgradeWall(ID id)
{
    auto& wall = wallRepository_.getWall(id);
    if (economyService_.tryUpgrade(wall.getUpgradableType(), wall.getLevel() + 1)) {
        wall.upgrade();
        return true;
    }
    return false;
}

bool WallService::destroyWall(Vector2<size_t> position)
{

    auto wallID = gridService_->atPoint(position).at(0);
    if (wallID == 0) {
        return false;
    }
    auto& wall = wallRepository_.getWall(wallID);
    if (economyService_.payDestruction(wall.getUpgradableType())) {
        wall.kill();
        gridService_->reset(position);
        worldMap_.setTileType(position, TileType::LAND);
#ifdef PARALLEL
        std::lock_guard lock(mutex_);
        #endif
        destroyed_.push_back(wallID);
        return true;
    }
    return false;
}

bool WallService::isKilled(ID id) const
{
    return std::ranges::find(destroyed_, id) != destroyed_.end();
}

void WallService::dealDamage(ID id, float damage)
{
    auto& wall = wallRepository_.getWall(id);
    wall.dealDamage(damage);
    if (!wall.isAlive()) {
        wall.kill();
#ifdef PARALLEL
        std::lock_guard lock(mutex_);
        #endif
        destroyed_.push_back(id);
    }
}

bool WallService::repairWall(ID id)
{
    auto& wall = wallRepository_.getWall(id);
    auto currentPercent = wall.getHealth() / wall.getMaxHealth();
    if (economyService_.tryRepair(wall.getUpgradableType(), wall.getLevel(), currentPercent)) {
        wall.repair(1 - currentPercent);
        return true;
    }
    return false;

}

void WallService::update(float dt) // does not collect destroyed
{
    wallRepository_.forEach([&](auto& wall)
    {
        wall.update(dt);
    });
}

std::vector<std::pair<ID, std::string>> WallService::collectDestroyed()
{
    std::ranges::for_each(destroyed_, [&](ID id)
    {
        economyService_.payDestruction(wallRepository_.getWall(id).getUpgradableType());

    });
    std::vector<std::pair<ID, std::string>> res{};

    for (auto id: destroyed_) {
        auto& wall = wallRepository_.getWall(id);
        res.emplace_back(id, wall.getUpgradableType().getName());
        gridService_->remove(wall.getPosition(), id);
        worldMap_.setTileType(wall.getPosition(), TileType::LAND);
        wallRepository_.removeWall(id);
    }

    destroyed_.clear();
    return res;
}



bool WallService::createWall(const std::string& wallName, Vector2<size_t> position)
{
    const auto id = idService_.getID();
    if (worldMap_.isEmpty(position) && economyService_.tryBuild(wallRepository_.getTypeRef(wallName)) && gridService_->add(id, position)){
        wallRepository_.addWall(id, wallFactoryManager_.create(wallName, position, id));
        worldMap_.setTileType(position, TileType::WALL);
        return true;
    }
    return false;
}


void WallService::reset()
{
    collectDestroyed();
    wallRepository_.forEach([&](auto& wall)
    {
        destroyed_.push_back(wall.getID());
    });
    collectDestroyed();
}

void WallService::toConfig(IConfigReader &config)
{
    wallRepository_.forEach([&](auto& wall)
    {
       wall.toConfig(config);
    });
}

void WallService::load(const IConfigReader &config)
{
    auto walls = config.getItems("");
    for (auto& [name_id, cfg] : walls) {
        auto it = std::find(name_id.rbegin(), name_id.rend(), '_');
        if (it == name_id.rend()) {
            throw std::runtime_error("WallService::load: Bad config: " + name_id);
        }
        size_t pos = name_id.rend() - it - 1;
        std::string suffix = name_id.substr(pos + 1);
        auto name = name_id.substr(0, pos);
        wallRepository_.addWall(cfg->getSize_t("id"), wallFactoryManager_.restore(name, *cfg));
        worldMap_.setTileType({cfg->getSize_t("x"), cfg->getSize_t("y")}, TileType::WALL);
        auto res = gridService_->add(cfg->getSize_t("id"), {cfg->getSize_t("x"), cfg->getSize_t("y")});
    }
}
