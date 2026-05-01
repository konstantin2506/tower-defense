#include "enemy.h"

#include <iostream>

using namespace TowerDefence;

void Enemy::move(Vector2<int> vector)
{
    constexpr float orthogonalMove = 10;
    constexpr float diagonalMove = 14;
    position_ = position_ + vector;
    if (std::abs(vector.getX()) + std::abs(vector.getY()) > 1) {

        moveComponent_.moveForDs(diagonalMove);
        return;
    }
    moveComponent_.moveForDs(orthogonalMove);

}

float Enemy::update(float dt) // return ds
{
    auto isTargeted = attack(dt);
    health_.update(dt);
    moveComponent_.setMoving(!isTargeted); //norm
    auto ds =  moveComponent_.update(dt); //norm
    abilityComponent_.tryUseAll(dt, *this);
    updateEffects(dt);
    return ds;
}

void Enemy::setFromConfig(const IConfigReader &config)
{
    moveComponent_.setMoving(config.getBool("is_moving"));
    moveComponent_.setSightVector({config.getInt("sight_x"), config.getInt("sight_y")});

    attackStrategy_->setFromConfig(*config.getNested("attack_strategy"));
    health_.setFromConfig(config);
    id_ = config.getSize_t("id");
    position_ = {config.getSize_t("x"), config.getSize_t("y")};
}

void Enemy::toConfig(IConfigReader &config) const
{
    auto name = type_.getName();
    name += "_" + std::to_string(id_);

    config.writeSize_tAt(name, "id", id_);
    config.writeSize_tAt(name, "x", position_.getX());
    config.writeSize_tAt(name, "y", position_.getY());

    config.writeIntAt(name, "sight_x", moveComponent_.getSightVector().getX());
    config.writeIntAt(name, "sight_y", moveComponent_.getSightVector().getY());

    config.writeFloatAt(name, "health", health_.getHealth());
    config.writeFloatAt(name, "max_health", health_.getMaxHealth());

    config.writeBoolAt(name, "is_moving", moveComponent_.isMoving());

    attackStrategy_->toConfig(name, config);
}
