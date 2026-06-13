#include "GridMovementComponent.h"
#include "GridCollisionComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include <cmath>

tron::GridMovementComponent::GridMovementComponent(dae::GameObject* owner, GridCollisionComponent* collision, float speed)
    : Component(owner)
    , m_Collision(collision)
    , m_Speed(speed)
    , m_pRenderComponent(owner->GetComponent<dae::RenderComponent>())
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
}

bool tron::GridMovementComponent::RequestMove(const glm::vec2& direction)
{
    if (!m_IsMoving)
    {
        StartMove(direction);
        return m_IsMoving;
    }
    return true;
}

void tron::GridMovementComponent::Stopmove()
{
    m_BufferedDirection = { 0.f, 0.f };
    m_CurrentDirection = { 0.f, 0.f };
}

bool tron::GridMovementComponent::CanMove(const glm::vec2& direction) const
{
    if (direction == glm::vec2{ 0.f, 0.f }) return false;

    const float tileSize = m_Collision->GetTileSize();
    const glm::vec3 collAnchor = GetFootprintAnchor();

    const float targetCollX = collAnchor.x + direction.x * tileSize;
    const float targetCollY = collAnchor.y + direction.y * tileSize;

    return CanMoveTo(targetCollX, targetCollY);
}

void tron::GridMovementComponent::Reset()
{
    m_IsMoving = false;
    m_CurrentDirection = { 0.f, 0.f };
    m_BufferedDirection = { 0.f, 0.f };
    m_TargetPosition = { 0.f, 0.f, 0.f };
}

void tron::GridMovementComponent::StartMove(const glm::vec2& direction)
{
    if (direction == glm::vec2{ 0.f, 0.f }) return;

    const float tileSize = m_Collision->GetTileSize();

    // Collision anchor of the current position
    const glm::vec3 collAnchor = GetFootprintAnchor();

    const float targetCollX = collAnchor.x + direction.x * tileSize;
    const float targetCollY = collAnchor.y + direction.y * tileSize;

    if (CanMoveTo(targetCollX, targetCollY))
    {
        m_TargetPosition = {
            targetCollX - m_CollisionOffset.x * tileSize,
            targetCollY - m_CollisionOffset.y * tileSize,
            0.f
        };
        m_CurrentDirection = direction;
        m_IsMoving = true;

        // Rotate sprite to match direction
        if (m_pRenderComponent)
        {
            if (direction.x > 0.f) m_pRenderComponent->SetAngle(0.f);           // right
            else if (direction.x < -0.f) m_pRenderComponent->SetAngle(180.f);   // left
            else if (direction.y > 0.f) m_pRenderComponent->SetAngle(90.f);     // down
            else                         m_pRenderComponent->SetAngle(270.f);   // up
        }
    }
}

bool tron::GridMovementComponent::CanMoveTo(float anchorX, float anchorY) const
{
    const float tileSize = m_Collision->GetTileSize();

    // Test all four tiles of the 2x2 footprint, each by its own center
    return m_Collision->IsWalkable(anchorX + tileSize * 0.5f, anchorY + tileSize * 0.5f) &&        // top-left tile
        m_Collision->IsWalkable(anchorX + tileSize * 1.5f, anchorY + tileSize * 0.5f) &&           // top-right tile
        m_Collision->IsWalkable(anchorX + tileSize * 0.5f, anchorY + tileSize * 1.5f) &&           // bottom-left tile
        m_Collision->IsWalkable(anchorX + tileSize * 1.5f, anchorY + tileSize * 1.5f);             // bottom-right tile
}

void tron::GridMovementComponent::SnapToTarget()
{
    auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
    if (transform)
        transform->SetLocalPosition(m_TargetPosition.x, m_TargetPosition.y);
}

glm::vec3 tron::GridMovementComponent::GetFootprintAnchor() const
{
    auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
    if (!transform) return {};

    const float tileSize = m_Collision->GetTileSize();
    const float offsetX = m_Collision->GetOffsetX();
    const float offsetY = m_Collision->GetOffsetY();
    const glm::vec3 pos = transform->GetLocalPosition(); // top-left of the 4x4 sprite

    // Shift inward by the collision offset to get the center 2x2 footprint anchor
    const float collX = pos.x + m_CollisionOffset.x * tileSize;
    const float collY = pos.y + m_CollisionOffset.y * tileSize;

    const int col = static_cast<int>(std::round((collX - offsetX) / tileSize));
    const int row = static_cast<int>(std::round((collY - offsetY) / tileSize));

    return {
        offsetX + col * tileSize,
        offsetY + row * tileSize,
        0.f
    };
}