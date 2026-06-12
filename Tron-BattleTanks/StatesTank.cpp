#include "StatesTank.h"
#include "GunComponent.h"
#include "GridCollisionComponent.h"
#include "GridMovementComponent.h"
#include "TransformComponent.h"
#include <cstdlib>
#include <array>

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
    // Pick an initial valid direction
    PickNewDirection(stateComponent);
}

std::unique_ptr<tron::TankState> tron::TankWanderState::Update(TankStateComponent& tank, float /*deltaTime*/)
{
    auto* movement = tank.GetMovement();

    if (movement && !movement->IsMoving())
    {
        // Try to keep moving in the current direction; if obstructed, pick a new one
        if (!movement->RequestMove(m_WanderDirection))
        {
            PickNewDirection(tank);
            movement->RequestMove(m_WanderDirection);
        }
    }

    // If a target comes into range, transition to shooting
    if (tank.IsTargetInRange()) {
        return std::make_unique<TankShootState>();
    }

    return nullptr;
}

void tron::TankWanderState::PickNewDirection(TankStateComponent& tank)
{
    auto* movement = tank.GetMovement();
    if (!movement) return;

    static constexpr std::array<glm::vec2, 4> directions = {
        glm::vec2{ 1.f, 0.f },
        glm::vec2{ -1.f, 0.f },
        glm::vec2{ 0.f, 1.f },
        glm::vec2{ 0.f, -1.f }
    };

    // Shuffle order so we don't always prefer the same direction
    std::array<int, 4> order = { 0, 1, 2, 3 };
    for (int i = 3; i > 0; --i)
    {
        const int j = std::rand() % (i + 1);
        std::swap(order[i], order[j]);
    }

    for (int idx : order)
    {
        if (movement->CanMove(directions[idx]))
        {
            m_WanderDirection = directions[idx];
            return;
        }
    }

    // No direction is currently free; keep the existing one (will retry next frame)
}