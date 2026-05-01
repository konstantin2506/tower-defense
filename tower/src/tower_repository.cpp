#include "tower_repository.h"

#include <iostream>

using namespace TowerDefence;

TowerRepository::TowerRepository(const IConfigReader& allTowersConfig)
{
    for (auto& [towerName, config] : allTowersConfig.getItems("towers")) {
        addType(towerName, *config);
    }
}

void TowerRepository::addType(const std::string& name, const IConfigReader& towerConfig)
{
    std::vector<std::unique_ptr<IConfigReader>> towerConfigs;
    towerConfigs.push_back(nullptr);
    for (auto& config : towerConfig.getItems("levels") | std::views::values) {
        towerConfigs.emplace_back(std::move(config));
    }
    towerTypes_.emplace(name, UpgradableType(name, std::move(towerConfigs)));
}

const UpgradableType & TowerRepository::getTypeRef(const std::string& name) const
{
    return towerTypes_.at(name);
}

void TowerRepository::forEach(std::function<void(ITower &)> callback) const
{
    for (auto& tower: allTowers_ | std::views::values) {
        callback(*tower);
    }
}

