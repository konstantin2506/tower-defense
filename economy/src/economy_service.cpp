#include "economy_service.h"

#include <iostream>

using namespace TowerDefence;

EconomyService::EconomyService(const IConfigReader& cfg) : startMoney_(cfg.getSize_t("money"))
{
    money_ = cfg.getSize_t("money");
    auto ssRewards = cfg.getSize_ts("rewards");
    for (auto& [name, money] : ssRewards) {
        rewards_.emplace(name, money);
    }
    for (auto& [name, config] :cfg.getItems("upgrades"))
    {
        auto maxLevel = config->getSize_t("max_level");

        auto costsConfig = config->getNested("costs");
        for (auto level = 1u; level <= maxLevel; ++level) {
            upgradeCosts_.emplace(std::make_pair(name, level), costsConfig->getSize_t(std::to_string(level)));
        }
    }
}

bool EconomyService::canUpgrade(const std::string& buildingType, Level level) const
{
    auto cost = upgradeCosts_.find({buildingType, level});
    if (cost == upgradeCosts_.end()) return false;
    return money_ >= cost->second ;
}

void EconomyService::rewardForKill(const std::string& enemyType)
{
    money_ += rewards_.at(enemyType);
}

bool EconomyService::tryUpgrade(const UpgradableType& buildingType, Level level)
{
    if (!canUpgrade(buildingType.getName(), level)) {
        return false;
    }
    money_ -= upgradeCosts_.at({buildingType.getName(), level});
    return true;
}

bool EconomyService::tryBuild(const UpgradableType& buildingType)
{
    return tryUpgrade(buildingType, 1);
}

bool EconomyService::tryRepair(const UpgradableType &buildingType, Level level, float currentPercent)
{
    auto it = upgradeCosts_.find({buildingType.getName(), level});
    if (it == upgradeCosts_.end())
        return false;
    auto cost = static_cast<Money>(static_cast<float>(it->second) * currentPercent);
    if (money_ >= cost) {
        money_ -= cost;
        return true;
    }
    return false;
}

bool EconomyService::payDestruction(const UpgradableType &buildingType)
{
    auto destructionFee = rewards_.at(buildingType.getName());
    if (money_ < destructionFee) {
        return false;
    }
    money_ -= destructionFee;
    return true;
}

void EconomyService::reset()
{
    money_ = startMoney_;
}



