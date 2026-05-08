#include "StatesTank.h"

std::unique_ptr<tron::TankState> tron::TankShootState::Update(Tank& tank)
{
    tank.Shoot();

    // If target leaves range, go back to wandering
    if (!tank.IsTargetInRange()) {
        return std::make_unique<TankWanderState>();
    }

    return nullptr;
}

std::unique_ptr<tron::TankState> tron::TankWanderState::Update(Tank& tank)
{
    tank.Wander();

    // If a target comes into range, transition to shooting
    if (tank.IsTargetInRange()) {
        return std::make_unique<TankShootState>();
    }

    return nullptr;
}
