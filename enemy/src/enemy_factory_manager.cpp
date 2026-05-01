#include "enemy_factory_manager.h"

using namespace TowerDefence;

void EnemyFactoryManager::init(const IConfigReader &config, const IEnemyRepository &repo)
{
    auto allConfigs = config.getItems("enemies");
    for (const auto& [name, cfg] : allConfigs) {
        addFactory(name, *cfg, repo);
    }
}
