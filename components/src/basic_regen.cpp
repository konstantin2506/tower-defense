#include "health_component.h"
#include "regen_components.h"

using namespace TowerDefence;

float BasicRegenable::calculateRegenSpeed(const HealthComponent &healthComponent)
{
    return coefficient_ * healthComponent.getMaxHealth() / 100; // 1 percent
}
