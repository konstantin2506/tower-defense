#include "ability.h"
#include "effects.h"

using namespace TowerDefence;

void DodgeAbility::tryUseAbility(float dt, IEnemy &enemy)
{
    timer_.update(dt);
    if (timer_.isPassed() && static_cast<int>(timer_.getPassed() * 1000 + timer_.getPassed() * 370) % static_cast<int>(101 - probability) == 0) {
        enemy.applyEffect(std::make_unique<AbsorbedDamageEffect<IEnemy>>(1, 0));
        timer_.reset();
    }
}

void DodgeAbility::setFromConfig(const IConfigReader &config)
{
    probability = config.getFloat("probability");
}


