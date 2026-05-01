#include "castle_service.h"

#include "castle.h"

using namespace TowerDefence;

CastleService::CastleService(ICastleRepository &repo, IIDService &iidService, IEconomyService &economyService, Vector2<size_t> castlePosition): castleRepository_(repo) , idService_(iidService), economyService_(economyService)
{
    auto& type = castleRepository_.getTypeRef("basic_castle");
    auto& startConfig = type.getConfig(1);
    auto castle_ = std::make_unique<Castle>(type, type.getMaxLevel(),
            startConfig.getFloat("health"), startConfig.getFloat("max_health"), std::make_unique<BasicRegenable>(startConfig.getFloat("regen")), idService_.getID(), castlePosition);
    castleRepository_.addCastle(std::move(castle_));
}

Vector2<size_t> CastleService::getPosition() const
{
    return castleRepository_.getCastle().getPosition();
}

bool CastleService::repairCastle()
{
    auto& castle = castleRepository_.getCastle();
    auto currentPercent = castle.getHealth() / castle.getMaxHealth();
    if (economyService_.tryRepair(castle.getUpgradableType(), castle.getLevel(), currentPercent)) {
        castle.repair(1 - currentPercent);
        return true;
    }
    return false;
}

bool CastleService::upgradeCastle()
{
    auto& castle = castleRepository_.getCastle();
    if (economyService_.tryUpgrade(castle.getUpgradableType(), castle.getLevel() + 1)) {
        castle.upgrade();

        return true;
    }
    return false;
}

void CastleService::dealDamage(float damage)
{
    castleRepository_.getCastle().dealDamage(damage);
}

ID CastleService::getCastleID() const
{
    auto& castle = castleRepository_.getCastle();
    return castle.getID();
}

void CastleService::update(float dt)
{
    castleRepository_.getCastle().update(dt);
}

void CastleService::reset()
{
    auto& type = castleRepository_.getTypeRef("basic_castle");
    auto& startConfig = type.getConfig(1);
    auto castle_ = std::make_unique<Castle>(type, type.getMaxLevel(),
    startConfig.getFloat("health"), startConfig.getFloat("max_health"), std::make_unique<BasicRegenable>(startConfig.getFloat("regen")), idService_.getID(), castleRepository_.getCastle().getPosition());

    castleRepository_.removeCastle();
    castleRepository_.addCastle(std::move(castle_));
}

void CastleService::toConfig(IConfigReader &config)
{
    castleRepository_.getCastle().toConfig(config);
}

void CastleService::load(const IConfigReader &config)
{
    auto& castle = castleRepository_.getCastle();
    castle.setFromConfig(config);
}
