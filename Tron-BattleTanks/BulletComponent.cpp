#include "BulletComponent.h"
#include "BulletBuilder.h"
#include "GridCollisionComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include <glm/gtc/constants.hpp>
#include <cmath>

tron::BulletComponent::BulletComponent(dae::GameObject* owner, GridCollisionComponent* collision, dae::Scene* scene, const glm::vec2& direction, float speed, float tileSize, float scale, float lifetime)
    : Component(owner)
    , m_Collision(collision)
    , m_pScene(scene)
    , m_pRenderComponent(owner->GetComponent<dae::RenderComponent>())
    , m_Direction(direction)
    , m_Speed(speed)
    , m_TileSize(tileSize)
    , m_Scale(scale)
    , m_TimeLeft(lifetime)
{
}

void tron::BulletComponent::Update(float deltaTime)
{
    auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
    if (!transform) return;

    m_TimeLeft -= deltaTime;
    if (m_TimeLeft <= 0.f)
    {
        tron::SpawnExplosion(*m_pScene, transform->GetLocalPosition(), m_TileSize, m_Scale);
        GetOwner()->MarkForDeath();
        return;
    }

    const glm::vec3 current = transform->GetLocalPosition();
    const float moveX = m_Direction.x * m_Speed * deltaTime;
    const float moveY = m_Direction.y * m_Speed * deltaTime;
    const float size = 12.f * m_Scale;

    glm::vec3 next = current;

    // Resolve X and Y independently so the bullet can bounce off walls
    if (CheckWall(m_Collision, current.x + moveX, current.y, size))
        m_Direction.x = -m_Direction.x;
    else
        next.x += moveX;

    if (CheckWall(m_Collision, current.x, current.y + moveY, size))
        m_Direction.y = -m_Direction.y;
    else
        next.y += moveY;

    transform->SetLocalPosition(next.x, next.y, next.z);

    if (m_pRenderComponent)
    {
        const float angle = std::atan2(m_Direction.y, m_Direction.x) * (180.f / glm::pi<float>());
        m_pRenderComponent->SetAngle(angle);
    }
}

bool tron::BulletComponent::CheckWall(tron::GridCollisionComponent* collision, float x, float y, float size) const
{
    const float center = size * 0.5f;
    return collision->IsWall(x + center, y + center);
}
