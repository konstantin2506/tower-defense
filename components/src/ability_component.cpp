#include "ability_component.h"

using namespace TowerDefence;

void AbilityComponent::addAbility(std::unique_ptr<IAbility> ability)
{
    abilities_.push_back(std::move(ability));
}

void AbilityComponent::tryUseAll(float dt, IEnemy& owner)
{
    std::ranges::for_each(abilities_, [&](auto& ability) {ability->tryUseAbility(dt, owner);});
}

std::vector<std::unique_ptr<IAbility>> AbilityComponent::getAllAbilities() const
{
    std::vector<std::unique_ptr<IAbility>> result;
    std::ranges::for_each(abilities_, [&](auto& ability)
    {
        result.emplace_back(std::move(ability->clone()));
    });
    return result;
}
