#include "GridMovementComponent.h"
#include "GridCollisionComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include <cmath>

tron::GridMovementComponent::GridMovementComponent(dae::GameObject* owner,
    GridCollisionComponent* collision,
    float speed)
    : Component(owner)
    , m_Collision(collision)
    , m_Speed(speed)
{
}

void tron::GridMovementComponent::Update(float deltaTime)
{
    auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
    if (!transform) return;

    if (m_IsMoving)
    {
        const glm::vec3 current = transform->GetLocalPosition();
        const glm::vec3 diff = m_TargetPosition - current;
        const float distLeft = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        const float step = m_Speed * deltaTime;

        if (step >= distLeft)
        {
            SnapToTarget();
            m_IsMoving = false;

            // Only continue if there is a buffered direction
            if (m_BufferedDirection != glm::vec2{ 0.f, 0.f })
            {
                StartMove(m_BufferedDirection);
                m_BufferedDirection = { 0.f, 0.f };
            }
        }
        else
        {
            const glm::vec3 moveDir = glm::vec3(diff.x, diff.y, 0.f) / distLeft;
            transform->SetLocalPosition(
                current.x + moveDir.x * step,
                current.y + moveDir.y * step
            );
        }
    }
    else
    {
        // Only move if there is a buffered direction
        if (m_BufferedDirection != glm::vec2{ 0.f, 0.f })
        {
            StartMove(m_BufferedDirection);
            m_BufferedDirection = { 0.f, 0.f };
        }
    }
}

void tron::GridMovementComponent::RequestMove(const glm::vec2& direction)
{
    if (!m_IsMoving)
        StartMove(direction);
    else
        m_BufferedDirection = direction; // buffer for next tile
}

void tron::GridMovementComponent::Stopmove()
{
    m_BufferedDirection = { 0.f, 0.f };
    m_CurrentDirection = { 0.f, 0.f };
}

void tron::GridMovementComponent::StartMove(const glm::vec2& direction)
{
    if (direction == glm::vec2{ 0.f, 0.f }) return;

    const glm::vec3 tileCenter = GetCurrentTileCenter();
    const float tileSize = m_Collision->GetTileSize();

    const float targetX = tileCenter.x + direction.x * tileSize;
    const float targetY = tileCenter.y + direction.y * tileSize;

    if (m_Collision->IsWalkable(targetX, targetY))
    {
        m_TargetPosition = { targetX, targetY, 0.f };
        m_CurrentDirection = direction;
        m_IsMoving = true;
    }
}

void tron::GridMovementComponent::SnapToTarget()
{
    auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
    if (transform)
        transform->SetLocalPosition(m_TargetPosition.x, m_TargetPosition.y);
}

glm::vec3 tron::GridMovementComponent::GetCurrentTileCenter() const
{
    auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
    if (!transform) return {};

    const float tileSize = m_Collision->GetTileSize();
    const float offsetX = m_Collision->GetOffsetX();
    const float offsetY = m_Collision->GetOffsetY();
    const glm::vec3 pos = transform->GetLocalPosition();

    // Snap to nearest tile center
    const int col = static_cast<int>((pos.x - offsetX) / tileSize);
    const int row = static_cast<int>((pos.y - offsetY) / tileSize);

    return {
        offsetX + col * tileSize + tileSize * 0.5f,
        offsetY + row * tileSize + tileSize * 0.5f,
        0.f
    };
}