#include "level_component.h"

using namespace TowerDefence;

void LevelComponent::setFromConfig(const IConfigReader &cfg)
{

    auto level = cfg.getSize_t("level");
    if (level > maxLevel_) {
        throw std::invalid_argument("Level number exceeds max level (bad config)");
    }
    currentLevel_ = level;
}
