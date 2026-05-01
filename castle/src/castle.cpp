#include "castle.h"

using namespace TowerDefence;

void Castle::upgrade()
{
    level_.incrementCurrentLevel();
    const auto& cfg = type_.getConfig(level_.getCurrentLevel());
    health_.setFromConfig(cfg);
}

void Castle::repair(float percent)
{
    if (isAlive()) {
        health_.dealHeal(health_.getMaxHealth() * percent);
    }
}

void Castle::update(float dt)
{
    health_.update(dt);
}

void Castle::setFromConfig(const IConfigReader &config)
{
    level_.setFromConfig(config);
    const auto& typeConfig = type_.getConfig(level_.getCurrentLevel());
    health_.setFromConfig(typeConfig);
    health_.setFromConfig(config);
    id_ = config.getSize_t("id");
    position_ = {config.getSize_t("x"), config.getSize_t("y")};
}

void Castle::toConfig(IConfigReader &config)
{
    config.writeSize_t("id", id_);
    config.writeSize_t("level", level_.getCurrentLevel());
    config.writeSize_t("x", position_.getX());
    config.writeSize_t("y", position_.getY());
    config.writeFloat("health", health_.getHealth());
    config.writeFloat("max_health", health_.getMaxHealth());
}
