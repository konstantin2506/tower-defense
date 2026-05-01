

#include "tower_factory.h"

#include "tower.h"
using namespace TowerDefence;

TowerFactory::TowerFactory(const UpgradableType& type, std::unique_ptr<IAttackStrategy> strategy)
{
    strategy->setFromConfig(*type.getConfig(1).getNested("attack_strategy"));
    donor_ = std::make_unique<Tower>(
        type,
        std::move(strategy),
        0,
        type.getMaxLevel(),
        Vector2<size_t>({0, 0})
    );

}

std::unique_ptr<ITower> TowerFactory::create(Vector2<size_t> position, ID id)
{
    auto tower = donor_->clone();
    tower->setPosition(position);
    tower->setID(id);
    return tower;
}

std::unique_ptr<ITower> TowerFactory::restore(const IConfigReader &cfg)
{
    auto restored = donor_->clone();
    restored->setFromConfig(cfg);
    return restored;
}
