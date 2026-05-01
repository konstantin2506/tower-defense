
#include "tower_factory_manager.h"

#include "attack_strategy_factory_manager.h"


using namespace TowerDefence;

void TowerFactoryManager::init(const IConfigReader& config, const ITowerRepository& repo)
{
    for (auto& [name, towerConfig] : config.getItems("towers")) {
        addFactory(name, *towerConfig, repo);
    }
}






