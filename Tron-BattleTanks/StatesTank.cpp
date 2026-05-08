#include "StatesTank.h"

void tron::TankShootState::OnEnter(TankStateComponent& stateComponent)
{
    stateComponent.ChangeToShootVisuals();
}

std::unique_ptr<tron::TankState> tron::TankShootState::Update(TankStateComponent& tank)
{
    tank.Shoot();

    // If target leaves range, go back to wandering
    if (!tank.IsTargetInRange()) {
        return std::make_unique<TankWanderState>();
    }

    return nullptr;
}

void tron::TankWanderState::OnEnter(TankStateComponent& stateComponent)
{
    stateComponent.ChangeToWanderVisuals();
}

std::unique_ptr<tron::TankState> tron::TankWanderState::Update(TankStateComponent& tank)
{
    tank.Wander();

    // If a target comes into range, transition to shooting
    if (tank.IsTargetInRange()) {
        return std::make_unique<TankShootState>();
    }

    return nullptr;
}
