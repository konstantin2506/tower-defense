#include "wall_factory.h"

#include "wall.h"

using namespace TowerDefence;

WallFactory::WallFactory(const UpgradableType &type)
{
    const auto& config = type.getConfig(1);
    donor_ = std::make_unique<Wall>(
        type, type.getMaxLevel(),
        config.getFloat("health"), config.getFloat("max_health"),
        std::make_unique<BasicRegenable>(config.getFloat("regen")), 0, Vector2<size_t>{0, 0} );
        }

std::unique_ptr<IWall> WallFactory::create(Vector2<size_t> position, ID id)
{
    auto wall = donor_->clone();
    wall->setPosition(position);
    wall->setID(id);
    return wall;
}

std::unique_ptr<IWall> WallFactory::restore(const IConfigReader &cfg)
{
    auto restored = donor_->clone();
    restored->setFromConfig(cfg);
    return restored;
}



