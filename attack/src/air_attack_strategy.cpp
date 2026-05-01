#include <algorithm>

#include "attack_strategies.h"

using namespace TowerDefence;

bool AirAttackStrategy::canAttack() const
{
    return timer_.isPassed() && ammo_ > 0;
}



bool AirAttackStrategy::tryAttack(IIDObject &attacker, float dt)
{
    timer_.update(dt);

    auto& enemy = dynamic_cast<IEnemy&>(attacker);
    auto pos = enemy.getPosition();
    if (pos == castleService_.getPosition()) {
        castleService_.dealDamage(enemy.getHealth() * castleCoefficient_);
        enemy.kill();
        return false;
    }

    if (canAttack()){
        auto position = attacker.getPosition();
        auto targets = wallService_.getInRadius(position, 0);
        if (!targets.empty() && targets[0] == 0) {
            return false;
        }
        wallService_.dealDamage(targets[0], damage_);
        --ammo_;
        timer_.reset();
        return true;
    }
    return false;

}

void AirAttackStrategy::setFromConfig(const IConfigReader &cfg)
{
    damage_ = cfg.getFloat("damage");
    timer_.setReloadTime( cfg.getFloat("reload_time"));
    ammo_ = cfg.getSize_t("ammo");
    castleCoefficient_ = cfg.getFloat("castle_coefficient");
}

void AirAttackStrategy::toConfig(const std::string &name, IConfigReader &config)
{
    config.writeFloatAtAt(name, "attack_strategy", "damage", damage_);
    config.writeFloatAtAt(name, "attack_strategy", "castle_coefficient", castleCoefficient_);
    config.writeFloatAtAt(name, "attack_strategy", "reload_time", timer_.getReloadTime());

    config.writeSize_tAtAt(name, "attack_strategy", "ammo", ammo_);
}



