#include "enemy_service.h"

#include <thread>

#include "grid_service.h"

using namespace TowerDefence;


EnemyService::EnemyService(IPathfindingService &pathfindingService, const AttackStrategyFactoryManager &attackManager,
    const AbilityFactoryManager &abilityManager, IEnemyRepository& repo, IIDService& idService, IEconomyService &economyService, std::unique_ptr<IGridService> gridService):
pathfindingService_(pathfindingService), economyService_(economyService) , idService_(idService), enemyRepository_(repo), enemyFactoryManager_(attackManager, abilityManager), gridService_(std::move(gridService))
{}

void EnemyService::createEnemy(const std::string &enemyName, Vector2<size_t> position)
{
    auto id = idService_.getID();

    enemyRepository_.addEnemy(id, enemyFactoryManager_.create(enemyName, position, id));
    try {
        if (!gridService_->add(id, position)) {
            throw std::runtime_error("Error: enemy not created: " + enemyName + std::string(" at position " + position.toString()));
        }
    }catch (std::exception &) {
        enemyRepository_.removeEnemy(id);
        throw;
    }
}

void EnemyService::restoreEnemy(const IConfigReader &cfg)
{
    auto id = cfg.getSize_t("id");
    auto position = Vector2<size_t>{cfg.getSize_t("x"), cfg.getSize_t("y")};
    const auto name = cfg.getString("name");
    enemyRepository_.addEnemy(id, enemyFactoryManager_.restore(name, cfg));
    try {
        if (!gridService_->add(id, position)) {
            throw std::runtime_error("Error: enemy not created: " + name + std::string(" at position " + position.toString()));
        }
    }catch (std::exception &) {
        enemyRepository_.removeEnemy(id);
        throw;
    }
}

void EnemyService::killEnemy(ID id)
{
    auto& enemy = enemyRepository_.getEnemy(id);
    enemy.kill();
    destroyed_.push_back(id);
}

void EnemyService::damageEnemy(ID id, float damage)
{
    auto& enemy = enemyRepository_.getEnemy(id);
    if (!enemy.isAlive()) {
        return;
    }
    enemy.dealDamage(damage);
    if (!enemy.isAlive()) {
        enemy.kill();
#ifdef PARALLEL
        std::lock_guard lock(mux_);
#endif

        destroyed_.push_back(id);
    }
}

void EnemyService::healEnemy(ID id, float heal)
{
    auto& enemy = enemyRepository_.getEnemy(id);
    enemy.dealHeal(heal);
}

Vector2<size_t> EnemyService::getPosition(ID id) const
{
    return enemyRepository_.getEnemy(id).getPosition();
}

std::vector<ID> EnemyService::getInRadius(Vector2<size_t> point, size_t radius) const
{
    return gridService_->getInRadius(point, radius);
}

void EnemyService::addEffect(ID id, std::unique_ptr<IEffect<IEnemy>> effect)
{
    enemyRepository_.getEnemy(id).applyEffect(std::move(effect));
}

void EnemyService::forEach(std::function<void(const IEnemy &)> callback) const
{
    enemyRepository_.forEach(callback);
}

void EnemyService::init(const IConfigReader &config)
{
    enemyFactoryManager_.init(config, enemyRepository_);
}

std::vector<std::pair<ID, std::string>> EnemyService::collectDestroyed()
{
    enemyRepository_.forEach([&](auto& enemy)
    {
       if (!enemy.isAlive() && (std::ranges::find(destroyed_, enemy.getID()) == destroyed_.end())) {
           destroyed_.push_back(enemy.getID());
       }
    });

    std::ranges::for_each(destroyed_, [&](auto& id)
    {
        economyService_.rewardForKill(enemyRepository_.getEnemy(id).getType().getName());
    });
    std::vector<std::pair<ID, std::string>> res{};

    for (auto id: destroyed_) {
        auto& enemy = enemyRepository_.getEnemy(id);
        res.emplace_back(id, enemy.getType().getName());

        gridService_->remove(enemy.getPosition(), id);
        enemyRepository_.removeEnemy(id);

    }
    destroyed_.clear();
    return res;
}

void EnemyService::update(float dt)
{
    #ifdef PARALLEL
    updateParallel(dt);

    #endif
    #ifndef PARALLEL


    enemyRepository_.forEach([&](IEnemy& enemy)
    {
        constexpr float orthogonalMove = 10;
        constexpr float diagonalMove = 14;

        float dsNeeded = 0;
        auto ds = enemy.update(dt);

        auto moveType = enemy.getMoveType();
        do {
            auto vector = pathfindingService_.getPath(moveType, enemy.getPosition());
            enemy.setSightVector(vector);
            dsNeeded = (abs(vector.getX()) + abs(vector.getY()) > 1) ? diagonalMove : orthogonalMove;
            if (enemy.getDs() >= dsNeeded) {
                gridService_->replace(enemy.getID(), enemy.getPosition(), enemy.getPosition() + vector);
                enemy.move(vector);
                ds -= dsNeeded;
            }else break;
        }while (ds > 0);
    });

    #endif
}

void EnemyService::updateParallel(float dt)
{
    std::vector<IEnemy*> enemies;
    enemyRepository_.forEach([&](IEnemy& enemy)
    {
        enemies.push_back(&enemy);
    });
    size_t numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) {
        numThreads = 4;
    }
    const size_t chunkSize = (enemies.size() + numThreads - 1) / numThreads;
    std::vector<std::thread> threads;

    auto processChunk = [&](size_t startIdx, size_t endIdx) {
        for (size_t i = startIdx; i < endIdx && i < enemies.size(); ++i) {
            IEnemy& enemy = *enemies[i];

            constexpr float orthogonalMove = 10;
            constexpr float diagonalMove = 14;

            float dsNeeded = 0;
            auto ds = enemy.update(dt);

            auto moveType = enemy.getMoveType();
            do {
                auto vector = pathfindingService_.getPath(moveType, enemy.getPosition());
                enemy.setSightVector(vector);
                dsNeeded = (abs(vector.getX()) + abs(vector.getY()) > 1) ? diagonalMove : orthogonalMove;

                if (enemy.getDs() >= dsNeeded) {
                    auto old_pos = enemy.getPosition();
                    enemy.move(vector);

                    gridService_->replace(enemy.getID(), old_pos, enemy.getPosition());

                    ds -= dsNeeded;
                } else {
                    break;
                }
            } while (ds > 0);
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

}

void EnemyService::reset()
{
    collectDestroyed();
    enemyRepository_.forEach([&](auto& enemy)
    {
        destroyed_.push_back(enemy.getID());
    });
    collectDestroyed();
}

void EnemyService::toConfig(IConfigReader &config)
{
    enemyRepository_.forEach([&](auto& enemy)
    {
        enemy.toConfig(config);
    });
}

void EnemyService::load(const IConfigReader &config)
{
    auto enemies = config.getItems("");
    for (auto& [name_id, cfg] : enemies) {
        auto it = std::find(name_id.rbegin(), name_id.rend(), '_');
        if (it == name_id.rend()) {
            throw std::runtime_error("EnemyService::load: Bad config: " + name_id);
        }
        size_t pos = name_id.rend() - it - 1;
        std::string suffix = name_id.substr(pos + 1);
        auto name = name_id.substr(0, pos);
        enemyRepository_.addEnemy(cfg->getSize_t("id"), enemyFactoryManager_.restore(name, *cfg));
        auto res = gridService_->add(cfg->getSize_t("id"), {cfg->getSize_t("x"), cfg->getSize_t("y")});
    }
}
