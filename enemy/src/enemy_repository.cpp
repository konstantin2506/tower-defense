#include "enemy_repository.h"

#include <algorithm>
#include <iostream>
#include <ranges>

using namespace TowerDefence;

EnemyRepository::EnemyRepository(const IConfigReader &allEnemyConfigs)
{
    for (auto& [name, config] : allEnemyConfigs.getItems("enemies")) {
        addType(name, *config);
    }
}

void EnemyRepository::addType(const std::string &name, const IConfigReader &config)
{
    std::cout << name << " added" << std::endl;
    static const std::unordered_map<std::string, MoveType> map = {
        {"landed", MoveType::LANDED},
        {"air", MoveType::AIR},
    };
    enemyTypes_.emplace_back(name, map.at(config.getString("move_type")));
}

const MovableType & EnemyRepository::getTypeRef(const std::string &name) const
{
    const auto it = std::ranges::find_if(enemyTypes_, [&](const Type& e) {
        return e.getName() == name;
    });
    return *it;

}

IEnemy & EnemyRepository::getEnemy(ID id) const
{
    return *allEnemies_.at(id);
}

void EnemyRepository::forEach(std::function<void(IEnemy &)> callback) const
{
    for (const auto& enemy: allEnemies_ | std::views::values) {callback(*enemy);}
}

void EnemyRepository::addEnemy(ID id, std::unique_ptr<IEnemy> enemy)
{
    allEnemies_.emplace(id, std::move(enemy));
}

void EnemyRepository::removeEnemy(ID id)
{
    allEnemies_.erase(id);
}
