#include "castle_repository.h"

#include <iostream>
#include <ranges>

#include "castle.h"

using namespace TowerDefence;

CastleRepository::CastleRepository(const IConfigReader& castleConfig)
{
    std::vector<std::unique_ptr<IConfigReader> > castleLevels;
    castleLevels.push_back(nullptr);
    for (auto &config: castleConfig.getItems("levels") | std::views::values ) {
        castleLevels.emplace_back(std::move(config));
    }
    castleType_ = UpgradableType{castleConfig.getString("name"), std::move(castleLevels)};
}

const UpgradableType & CastleRepository::getTypeRef(const std::string &name) const
{
    return castleType_;
}

ICastle & CastleRepository::getCastle() const
{
    return *castle_;
}

void CastleRepository::removeCastle()
{
    castle_ = nullptr;
}
