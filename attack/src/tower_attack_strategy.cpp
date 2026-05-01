#include <algorithm>
#include <iostream>

#include "attack_strategies.h"

using namespace TowerDefence;

bool TowerAttackStrategy::tryAttack(IIDObject& attacker, float dt)
{

    timer_.update(dt);
    if (canAttack()){

        auto position = attacker.getPosition();
        auto enemies = enemyService_.getInRadius(position, radius_);

        if (enemies.empty()) {
            return false;
        }
        auto nearest = *std::ranges::min_element(enemies, std::less{}, [&](ID enemy) {
            return Vector2<size_t>::distanceQ(position, enemyService_.getPosition(enemy));
        });

        enemyService_.damageEnemy(nearest, damage_);
        timer_.reset();

    }
    return true;
}

void TowerAttackStrategy::setFromConfig(const IConfigReader &cfg)
{
    radius_ = cfg.getSize_t("radius");
    damage_ = cfg.getFloat("damage");
    timer_.setReloadTime( cfg.getFloat("reload_time"));
}

bool TowerAttackStrategy::canAttack() const
{
    return timer_.isPassed();
}

void TowerAttackStrategy::toConfig(const std::string &name, IConfigReader &config)
{
    config.writeSize_tAtAt(name, "attack_strategy", "radius", radius_);
    config.writeFloatAtAt(name, "attack_strategy", "damage", damage_);
    config.writeFloatAtAt(name, "attack_strategy", "reload_time", timer_.getReloadTime());
}
