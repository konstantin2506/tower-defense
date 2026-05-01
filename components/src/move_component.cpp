#include "move_component.h"

using namespace TowerDefence;

float MoveComponent::update(float dt)
{
    if (!isMoving_) {
        return 0;
    }
    addDs(dt);
    return ds_;
}
