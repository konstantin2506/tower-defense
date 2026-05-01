#include "enemy_factory.h"

#include <ranges>

#include "enemy.h"

using namespace TowerDefence;

EnemyFactory::EnemyFactory(const MovableType& type, std::unique_ptr<IAttackStrategy> strategy, std::vector<std::unique_ptr<IAbility>> abilities, const IConfigReader& config)
{
    strategy->setFromConfig(*config.getNested("attack_strategy"));
    
    donor_ = std::make_unique<Enemy>(
        type, config.getFloat("health"), config.getFloat("max_health"), std::make_unique<BasicRegenable>(config.getFloat("regen")),
        std::move(strategy), std::move(abilities), Vector2<size_t>{0, 0}, 0,
        config.getFloat("velocity")
        );

}

std::unique_ptr<IEnemy> EnemyFactory::create(Vector2<size_t> position, ID id) const
{
    auto enemy = donor_->clone();
    enemy->setPosition(position);
    enemy->setID(id);
    return enemy;
}

std::unique_ptr<IEnemy> EnemyFactory::restore(const IConfigReader &cfg) const
{
    auto restored = donor_->clone();
    restored->setFromConfig(cfg);
    return restored;
}
