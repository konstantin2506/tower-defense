#include "game_loader.h"

#include <fstream>
#include <iostream>

#include "ability_factories.h"
#include "attack_strategy_factories.h"
#include "attack_strategy_factory_manager.h"
#include "castle_repository.h"
#include "castle_service.h"
#include "economy_service.h"
#include "id_service.h"
#include "world_map.h"
#include "pathfinding.h"
#include "enemy_repository.h"
#include "enemy_service.h"
#include "grid_service.h"
#include "tower_repository.h"
#include "tower_service.h"
#include "wall_repository.h"
#include "wall_service.h"
#include "../../effects/include/effects.h"

using namespace TowerDefence;


GameLoader::GameLoader(const IConfigReader& configOfConfigs)
{
    auto configPaths = configOfConfigs.getStrings("configs");

    for (const auto& [name, configPath] : configPaths) {
        allConfigs_.emplace(name, configOfConfigs.fromFile(configPath));

    }
    Vector2<size_t> castlePosition{};
    try {
        auto x = allConfigs_.at("castle_repository")->getSize_t("x_coord");
        auto y = allConfigs_.at("castle_repository")->getSize_t("y_coord");
        castlePosition = Vector2<size_t>(x, y);
    }catch (const std::exception& e) {
        std::cerr << "Error in castleRepository init no position: ";
        std::cerr << e.what() << std::endl;
    }

    attackManager_ = std::make_unique<AttackStrategyFactoryManager>();
    abilityManager_ = std::make_unique<AbilityFactoryManager>();
    idService_ = std::make_unique<IDService>();

    try {
        economyService_ = std::make_unique<EconomyService>(*allConfigs_.at("economy_service"));
    }catch (const std::exception& e) {
        std::cerr << "Error in economyService init: ";
        std::cerr << e.what() << std::endl;
    }
    try {
        worldMap_ = std::make_unique<WorldMap>(*allConfigs_.at("world_map"));
        worldMap_->setTileType(castlePosition, TileType::CASTLE);
    }catch (const std::exception& e) {
        std::cerr << "Error in worldMap init: ";
        std::cerr << e.what() << std::endl;
    }
    try {
        pathfindingService_ = std::make_unique<PathfindingService>(*allConfigs_.at("pathfinding"), *worldMap_, castlePosition);
    }catch (const std::exception& e) {
        std::cerr << "Error in pathfindingService init: ";
        std::cerr << e.what() << std::endl;
    }
        auto worldSize = worldMap_->size();
        auto worldSizeX = worldSize.getX();
        auto worldSizeY = worldSize.getY();
    try {
        enemyRepository_ = std::make_unique<EnemyRepository>(*allConfigs_.at("enemy_repository"));
    }
    catch (const std::exception& e) {
        std::cerr << "Error in enemyRepository init: ";
        std::cerr << e.what() << std::endl;
    }
    try {
        wallRepository_ = std::make_unique<WallRepository>(*allConfigs_.at("wall_repository"));
    }catch (const std::exception& e) {
        std::cerr << "Error in wallRepository init: ";
        std::cerr << e.what() << std::endl;
    }
    try {
        towerRepository_ = std::make_unique<TowerRepository>(*allConfigs_.at("tower_repository"));
    }catch (const std::exception& e) {
        std::cerr << "Error in towerRepository init: ";
        std::cerr << e.what() << std::endl;
    }
    try {
        castleRepository_ = std::make_unique<CastleRepository>(*allConfigs_.at("castle_repository"));
    }catch (const std::exception& e) {
        std::cerr << "Error in castleRepository init: ";
        std::cerr << e.what() << std::endl;
    }
    try {
        castleService_ = std::make_unique<CastleService>(*castleRepository_, *idService_, *economyService_,  castlePosition);
    }catch (const std::exception& e) {
        std::cerr << "Error in castleService init: ";
        std::cerr << e.what() << std::endl;
    }

    wallService_ = std::make_unique<WallService>(*idService_, *economyService_, *wallRepository_, *worldMap_, std::make_unique<SingleIDGridService>(worldSizeX, worldSizeY));
    towerService_ = std::make_unique<TowerService>(*towerRepository_, *idService_, *economyService_, *worldMap_, *attackManager_, std::make_unique<SingleIDGridService>(worldSizeX, worldSizeY));
    enemyService_ = std::make_unique<EnemyService>(*pathfindingService_, *attackManager_, *abilityManager_, *enemyRepository_, *idService_, *economyService_, std::make_unique<VecIDGridService>(worldSizeX, worldSizeY));

    init(allConfigs_);

    try {
        view_ = std::make_unique<View>(*allConfigs_.at("view"), *worldMap_, buildings);
    }catch (const std::exception& e) {
        std::cerr << "Error in view init: ";
        std::cerr << e.what() << std::endl;
    }

    model_ = std::make_unique<Model>(*allConfigs_.at("enemy_service_save"),
                                     *allConfigs_.at("wall_service_save"),
                                     *allConfigs_.at("tower_service_save"),
                                     *allConfigs_.at("castle_service_save"),
                                     *allConfigs_.at("economy_service_save"),
                                     *allConfigs_.at("id_service_save"),
                                     *enemyService_, *wallService_, *towerService_, *castleService_,
                                     *worldMap_, *enemyRepository_, *wallRepository_, *towerRepository_, *castleRepository_,
                                     *economyService_, *pathfindingService_, *idService_);
}



void GameLoader::init(const std::unordered_map<std::string, std::unique_ptr<IConfigReader>>& configs)
{
    try {
        initAttacks(*configs.at("attack_manager"));
    }catch (const std::exception& e) {
        std::cerr << "Error in initAttacks init: ";
        std::cerr << e.what() << std::endl;
    }
    try {
        initAbilities(*configs.at("ability_manager"));
    }catch (const std::exception& e) {
        std::cerr << "Error in initAbilities init: ";
        std::cerr << e.what() << std::endl;
    }


    try {

        enemyService_->init(*configs.at("enemy_service"));
    }catch (const std::exception& e) {
        std::cerr << "Error in enemyService init: ";
        std::cerr << e.what() << std::endl;
    }
    try {
        wallService_->init(*configs.at("wall_service"));
    }catch (const std::exception& e) {
        std::cerr << "Error in wallService init: ";
        std::cerr << e.what() << std::endl;
    }
    try {
        towerService_->init(*configs.at("tower_service"));
    }catch (const std::exception& e) {
        std::cerr << "Error in towerService init: ";
        std::cerr << e.what() << std::endl;
    }
}

//по идее если все ок просто дописываем вот в эти методы новые абилки и атаки

void GameLoader::initAbilities(const IConfigReader& config)
{
    abilityManager_->addFactory<DodgeAbility>("dodge", config, *enemyService_);
    abilityManager_->addFactory<DieDropAbility>("die_drop", config, *enemyService_, *worldMap_);

    abilityManager_->addFactory<AuraAbility<IEnemy, IEnemyService, VelocityEffect<IEnemy>>>("enemy_speed_aura", config, *enemyService_, *enemyService_);
    abilityManager_->addFactory<AuraAbility<IEnemy, IEnemyService, MaxHealthEffect<IEnemy>>>("enemy_max_health_aura", config, *enemyService_, *enemyService_);
    abilityManager_->addFactory<AuraAbility<IEnemy, IEnemyService, AbsorbedHealEffect<IEnemy>>>("enemy_heal_aura", config, *enemyService_, *enemyService_);
    abilityManager_->addFactory<AuraAbility<IEnemy, IEnemyService, AbsorbedDamageEffect<IEnemy>>>("enemy_absorbed_damage_aura", config, *enemyService_, *enemyService_);

    abilityManager_->addFactory<AuraAbility<IWall, IWallService, MaxHealthEffect<IWall>>>("wall_max_health_aura", config, *enemyService_, *wallService_);
    abilityManager_->addFactory<AuraAbility<IWall, IWallService, AbsorbedHealEffect<IWall>>>("wall_heal_aura", config, *enemyService_, *wallService_);
    abilityManager_->addFactory<AuraAbility<IWall, IWallService, AbsorbedDamageEffect<IWall>>>("wall_absorbed_damage_aura", config, *enemyService_, *wallService_);

}

void GameLoader::initAttacks(const IConfigReader &config)
{
    attackManager_->addFactory<TowerAttackStrategy>("tower_range", config, *enemyService_);
    attackManager_->addFactory<RangeEnemyAttackStrategy>("enemy_range", config, *wallService_, *pathfindingService_, *castleService_);
    attackManager_->addFactory<AirAttackStrategy>("enemy_air", config, *wallService_, *castleService_);
}




