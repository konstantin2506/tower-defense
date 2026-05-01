#include <ranges>

#include "ability.h"

using namespace TowerDefence;

void DieDropAbility::tryUseAbility(float dt, IEnemy &owner)
{
    if (!owner.isAlive()) {
        int x = 0;
        const auto pos = owner.getPosition();
        for (const auto &enemy: enemiesInside_) {
            auto position = pos;
            position.setX(pos.getX() + x);
            position.setY(pos.getY() - x + 1);
            if (position.getX() >= world_.size().getX()-1 || pos.getY() >= world_.size().getY()-1) {
                position = pos;
            }
            enemyService_.createEnemy(enemy, position);
            x = -x+1;
        }
    }
}

void DieDropAbility::setFromConfig(const IConfigReader &config)
{
    auto insideString = config.getString("enemies_inside");
    auto split = std::views::split(insideString, ',');

    std::vector<std::string> enemies;
    for (auto&& range : split) {
        enemies.emplace_back(range.begin(), range.end());
    }
    std::swap(enemies, enemiesInside_);
}
