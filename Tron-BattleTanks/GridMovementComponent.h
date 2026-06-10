#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace tron
{
    class GridCollisionComponent;

    class GridMovementComponent final : public dae::Component
    {
    public:
        GridMovementComponent(dae::GameObject* owner,
            GridCollisionComponent* collision,
            float speed);

        void Update(float deltaTime) override;

        void RequestMove(const glm::vec2& direction);

        bool IsMoving() const { return m_IsMoving; }
        void Stopmove();

    private:
        GridCollisionComponent* m_Collision;
        float m_Speed;

        const int m_PlayerSize = 2; // in tiles 2x2

        glm::vec2 m_CurrentDirection{ 0.f, 0.f };
        glm::vec2 m_BufferedDirection{ 0.f, 0.f };

        glm::vec2 m_CollisionOffset{ 1.f,1.f };

        glm::vec3 m_TargetPosition{ 0.f };
        bool m_IsMoving{ false };

        void StartMove(const glm::vec2& direction);
        void SnapToTarget();
        bool CanMoveTo(float centerX, float centerY) const;
        glm::vec3 GetFootprintAnchor() const;
    };
}