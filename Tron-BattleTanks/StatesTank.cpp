#include "StatesTank.h"
#include "GunComponent.h"
#include "GridCollisionComponent.h"
#include "GridMovementComponent.h"
#include "TransformComponent.h"
#include <cstdlib>

void tron::TankShootState::OnEnter(TankStateComponent& stateComponent)
{
}

std::unique_ptr<tron::TankState> tron::TankShootState::Update(TankStateComponent& tank, float /*deltaTime*/)
{
    // Shooting logic lives here
    const glm::vec2 dir = tank.GetDirectionToNearestTarget();
    auto* gun = tank.GetGun();
    if (gun && (dir.x != 0.f || dir.y != 0.f))
        gun->Shoot(dir);

    // If target leaves range, go back to wandering
    if (!tank.IsTargetInRange()) {
        return std::make_unique<TankWanderState>();
    }

    return nullptr;
}

void tron::TankWanderState::OnEnter(TankStateComponent& stateComponent)
{
    m_DirectionChangeTimer = 0.f;
}

std::unique_ptr<tron::TankState> tron::TankWanderState::Update(TankStateComponent& tank, float deltaTime)
{
    // Wandering logic lives here, using grid-snapped movement
    auto* movement = tank.GetMovement();

    m_DirectionChangeTimer += deltaTime;

    if (movement && !movement->IsMoving())
    {
        if (m_DirectionChangeTimer >= m_DirectionChangeInterval)
        {
            const int dir = std::rand() % 4;
            switch (dir)
            {
            case 0: m_WanderDirection = { 1.f, 0.f };  break;
            case 1: m_WanderDirection = { -1.f, 0.f }; break;
            case 2: m_WanderDirection = { 0.f, 1.f };  break;
            case 3: m_WanderDirection = { 0.f, -1.f }; break;
            }
            m_DirectionChangeTimer = 0.f;
        }

        movement->RequestMove(m_WanderDirection);
    }

    // If a target comes into range, transition to shooting
    if (tank.IsTargetInRange()) {
        return std::make_unique<TankShootState>();
    }

    return nullptr;
}