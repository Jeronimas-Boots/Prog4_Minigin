#pragma once
#include "Command.h"
#include "GridMovementComponent.h"
#include <glm/glm.hpp>

namespace tron
{
    class GridMoveCommand final : public dae::Command
    {
    public:
        GridMoveCommand(GridMovementComponent* movement, const glm::vec2& direction)
            : m_Movement(movement)
            , m_Direction(direction)
        {
        }

        void Execute(float) override
        {
            m_Movement->RequestMove(m_Direction);
        }

    private:
        GridMovementComponent* m_Movement;
        glm::vec2 m_Direction;
    };
}