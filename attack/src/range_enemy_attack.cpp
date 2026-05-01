#include <algorithm>

#include "attack_strategies.h"

using namespace TowerDefence;

bool RangeEnemyAttackStrategy::canAttack() const
{
    return currentTarget_ != 0  && !wallService_.isKilled(currentTarget_) && timer_.isPassed();
}
bool RangeEnemyAttackStrategy::tryAttack(IIDObject &attacker, float dt)
{
    constexpr auto diagonal = 1.4f;
    constexpr auto orthogonal = 1.f;

    auto& enemy = dynamic_cast<IEnemy&>(attacker);

    timer_.update(dt);
    auto moveType = enemy.getMoveType();
    float ds = static_cast<float>(radius_);

    auto pos = enemy.getPosition();
    if (pos == castleService_.getPosition()) {
        castleService_.dealDamage(enemy.getHealth() * castleCoefficient_);
        enemy.kill();
        return false;
    }

    while (ds > 0) {
        auto path = pathfinding_.getPath(moveType, pos);
        auto length = (std::abs(path.getX()) + std::abs(path.getY()) > 1) ? diagonal : orthogonal;

        if (length < ds) {
            ds -= length;
            pos = pos + path;
        } else {
            break;
        }
        auto target = wallService_.getInRadius(pos, 0);
        if (!target.empty() && target[0] != 0) {
            currentTarget_ = target[0];
            break;
        }
        currentTarget_ = 0;
    }

    if (canAttack() ){
        wallService_.dealDamage(currentTarget_, damage_);
        timer_.reset();
    }
    return currentTarget_ > 0;
}
void RangeEnemyAttackStrategy::setFromConfig(const IConfigReader &cfg)
{
    radius_ = cfg.getSize_t("radius");
    damage_ = cfg.getFloat("damage");
    timer_.setReloadTime( cfg.getFloat("reload_time"));
}

void RangeEnemyAttackStrategy::toConfig(const std::string &name, IConfigReader &config)
{
    config.writeFloatAtAt(name, "attack_strategy", "damage", damage_);
    config.writeFloatAtAt(name, "attack_strategy", "reload_time", timer_.getReloadTime());

    config.writeSize_tAtAt(name, "attack_strategy", "radius", radius_);
}


