#include "health_component.h"

using namespace TowerDefence;

void HealthComponent::dealDamage(float damage)
{
#ifndef PARALLEL
    health_ = std::max(.0f, health_ - damage * absorbedDamageCoefficient_);
#else
    float damageCoeff = absorbedDamageCoefficient_.load(std::memory_order_relaxed);
    float actualDamage = damage * damageCoeff;

    float expected = health_.load(std::memory_order_relaxed);
    float desired;

    do {
        desired = std::max(0.0f, expected - actualDamage);
    } while (!health_.compare_exchange_weak(expected, desired, std::memory_order_relaxed, std::memory_order_relaxed));
#endif
}

void HealthComponent::dealHeal(float heal)
{
#ifndef PARALLEL
    if (isAlive()){
        health_ = std::min(maxHealth_ * maxHealthCoefficient_, health_ + heal * absorbedHealCoefficient_);
    }
#else
    if (isAlive()) {
        float healCoeff = absorbedHealCoefficient_.load(std::memory_order_relaxed);
        float maxHealthCoeff = maxHealthCoefficient_.load(std::memory_order_relaxed);
        float actualHeal = heal * healCoeff;
        float currentMaxHealth = maxHealth_.load(std::memory_order_relaxed);
        float maxAllowed = currentMaxHealth * maxHealthCoeff;

        float expected = health_.load(std::memory_order_relaxed);
        float desired;

        do {
            desired = expected + actualHeal;
            if (desired > maxAllowed) desired = maxAllowed;
        } while (!health_.compare_exchange_weak(expected, desired, std::memory_order_relaxed, std::memory_order_relaxed));
    }
#endif
}

void HealthComponent::update(float dt)
{
    dealHeal(regen_->calculateRegenSpeed(*this) * absorbedHealCoefficient_ * dt);
}

void HealthComponent::setFromConfig(const IConfigReader& config)
{
    auto health = config.getFloat("health");
    maxHealth_ = config.getFloat("max_health");
    if (health > maxHealth_) {
        throw std::invalid_argument("Max health value exceeded (bad config)");
    }
    health_ = health;
}
