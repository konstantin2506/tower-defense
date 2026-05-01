
#include "tower_service.h"

#include <thread>

#include "grid_service.h"
using namespace TowerDefence;

bool TowerService::upgradeTower(ID towerID) const
{
    auto& tower = towerRepository_.getTower(towerID);
    if (economyService_.tryUpgrade(tower.getUpgradableType(), tower.getLevel() + 1)) {
        tower.upgrade();
        return true;
    }
    return false;
}

void TowerService::update(float dt)
{
#ifndef PARALLEL

    towerRepository_.forEach([&](auto& tower)
     {
         tower.update(dt);
     });


#else
    std::vector<ITower*> towers;
    towerRepository_.forEach([&](ITower& tower)
    {
        towers.push_back(&tower);
    });
    size_t numThreads = std::thread::hardware_concurrency();

    const size_t chunkSize = (towers.size() + numThreads - 1) / numThreads;
    std::vector<std::thread> threads;

    auto processChunk = [&](size_t startIdx, size_t endIdx) {
        for (size_t i = startIdx; i < endIdx && i < towers.size(); ++i) {
            ITower& tower = *towers[i];

            tower.update(dt);
        }
    };

    for (size_t t = 0; t < numThreads; ++t) {
        size_t start = t * chunkSize;
        size_t end = start + chunkSize;
        threads.emplace_back(processChunk, start, end);
    }


    for (auto& thread : threads) {
        thread.join();
    }

#endif


}

bool TowerService::destroyTower(Vector2<::size_t> position)
{
    auto id = gridService_->atPoint(position).at(0);
    if (id == 0) {
        return false;
    }
    gridService_->reset(position);
    worldMap_.setTileType(position, TileType::LAND);
    #ifdef PARALLEL
    std::lock_guard lock(mux_);
    #endif
    destroyed_.push_back(id);
    return true;
}

std::vector<std::pair<ID, std::string>> TowerService::collectDestroyed()
{
    std::ranges::for_each(destroyed_, [&](auto& id)
    {
        economyService_.payDestruction(towerRepository_.getTower(id).getUpgradableType());
    });
    std::vector<std::pair<ID, std::string>> res{};

    for (auto id: destroyed_) {
        auto& tower = towerRepository_.getTower(id);
        res.emplace_back(id, tower.getUpgradableType().getName());
        gridService_->remove(tower.getPosition(), id);
        worldMap_.setTileType(tower.getPosition(), TileType::LAND);
        towerRepository_.removeTower(id);
    }
    destroyed_.clear();
    return res;
}

void TowerService::reset()
{
    collectDestroyed();
    towerRepository_.forEach([&](auto& tower)
    {
        destroyed_.push_back(tower.getID());
    });
    collectDestroyed();
}

void TowerService::toConfig(IConfigReader &config)
{
    towerRepository_.forEach([&](auto& tower)
    {
       tower.toConfig(config);
    });
}

void TowerService::load(const IConfigReader &config)
{
    auto towers = config.getItems("");
    for (auto& [name_id, cfg] : towers) {
        auto it = std::find(name_id.rbegin(), name_id.rend(), '_');
        if (it == name_id.rend()) {
            throw std::runtime_error("TowerService::load: Bad config: " + name_id);
        }
        size_t pos = name_id.rend() - it - 1;
        std::string suffix = name_id.substr(pos + 1);
        auto name = name_id.substr(0, pos);
        towerRepository_.addTower(cfg->getSize_t("id"), towerFactoryManager_.restore(name, *cfg));
        auto res = gridService_->add(cfg->getSize_t("id"), {cfg->getSize_t("x"), cfg->getSize_t("y")});
        worldMap_.setTileType({cfg->getSize_t("x"), cfg->getSize_t("y")}, TileType::TOWER);
    }
}

void TowerService::init(const IConfigReader& config)
{
    towerFactoryManager_.init(config, towerRepository_);
}

TowerService::TowerService(ITowerRepository &towerRepository, IIDService &idService, IEconomyService &economyService,
    IWorldMap &worldMap, const AttackStrategyFactoryManager& asfm, std::unique_ptr<IGridService> gridService):
    towerRepository_(towerRepository), idService_(idService), economyService_(economyService), worldMap_(worldMap), towerFactoryManager_(asfm), gridService_(std::move(gridService))
{}

bool TowerService::createTower(const std::string& towerName, Vector2<size_t> position)
{
    const auto id = idService_.getID();
    if (worldMap_.isEmpty(position) && economyService_.tryBuild(towerRepository_.getTypeRef(towerName)) && gridService_->add(id, position)){
        towerRepository_.addTower(id, towerFactoryManager_.create(towerName, position, id));
        worldMap_.setTileType(position, TileType::TOWER);
        return true;
    }
    return false;
}

