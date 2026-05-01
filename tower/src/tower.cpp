#include "tower.h"

#include <iostream>

void TowerDefence::Tower::upgrade()
{
    level_.incrementCurrentLevel();
    const auto& cfg = type_.getConfig(level_.getCurrentLevel());
    attackStrategy_->setFromConfig(*cfg.getNested("attack_strategy"));
}

void TowerDefence::Tower::update(float dt)
{
    attack(dt);
    updateEffects(dt);
}

void TowerDefence::Tower::setFromConfig(const IConfigReader &config)
{
    level_.setFromConfig(config);
    const auto& typeConfig = type_.getConfig(level_.getCurrentLevel());
    attackStrategy_->setFromConfig(*typeConfig.getNested("attack_strategy"));
    id_ = config.getSize_t("id");
    position_ = {config.getSize_t("x"), config.getSize_t("y")};
}

void TowerDefence::Tower::toConfig(IConfigReader &config)
{
    auto name = type_.getName();
    name += "_" + std::to_string(id_);

    config.writeSize_tAt(name, "id", id_);
    config.writeSize_tAt(name, "level", level_.getCurrentLevel());
    config.writeSize_tAt(name, "x", position_.getX());
    config.writeSize_tAt(name, "y", position_.getY());
    attackStrategy_->toConfig(name, config);
}