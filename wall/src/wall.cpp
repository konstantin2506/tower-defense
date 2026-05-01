#include "wall.h"

void TowerDefence::Wall::repair(float percent) // на сколько процентов в долях надо реснуть
{
    if (isAlive()) {
        health_.dealHeal(health_.getMaxHealth() * percent);
    }
}

void TowerDefence::Wall::upgrade()
{
    level_.incrementCurrentLevel();
    const auto& cfg = type_.getConfig(level_.getCurrentLevel());
    health_.setFromConfig(cfg);
}

void TowerDefence::Wall::setFromConfig(const IConfigReader &config)
{
    level_.setFromConfig(config);
    const auto& typeConfig = type_.getConfig(level_.getCurrentLevel());
    health_.setFromConfig(typeConfig);
    health_.setFromConfig(config);
    id_ = config.getSize_t("id");
    position_ = {config.getSize_t("x"), config.getSize_t("y")};
}

void TowerDefence::Wall::update(float dt)
{
    health_.update(dt);
    updateEffects(dt);
}

void TowerDefence::Wall::toConfig(IConfigReader &config)
{
    auto name = type_.getName();
    name += "_" + std::to_string(id_);

    config.writeSize_tAt(name, "id", id_);
    config.writeSize_tAt(name, "level", level_.getCurrentLevel());
    config.writeSize_tAt(name, "x", position_.getX());
    config.writeSize_tAt(name, "y", position_.getY());
    config.writeFloatAt(name, "health", health_.getHealth());
    config.writeFloatAt(name, "max_health", health_.getMaxHealth());
}
