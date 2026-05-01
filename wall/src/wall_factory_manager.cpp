#include "wall_factory_manager.h"

#include <ranges>

using namespace TowerDefence;



void WallFactoryManager::init(const IConfigReader &config, const IWallRepository &repo)
{
    for (auto& name : config.getItems("walls") | std::views::keys) {
        addFactory(name, repo);
    }
}
