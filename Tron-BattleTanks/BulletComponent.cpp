#include "BulletComponent.h"
#include "BulletBuilder.h"
#include "GridCollisionComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "RectColliderComponent.h"
#include "HealthComponent.h"
#include <glm/gtc/constants.hpp>
#include <cmath>
#include "EventID.h"
#include "ScoreComponent.h"

tron::BulletComponent::BulletComponent(dae::GameObject* owner, GridCollisionComponent* collision,
    dae::Scene* scene, const glm::vec2& direction, float speed, float tileSize,
    float scale, float lifetime, const dae::GameObject* shooter,
    std::vector<dae::GameObject*> targets)
    : Component(owner)
    , m_Collision(collision)
    , m_pScene(scene)
    , m_pRenderComponent(owner->GetComponent<dae::RenderComponent>())
    , m_Direction(direction)
    , m_Speed(speed)
    , m_TileSize(tileSize)
    , m_Scale(scale)
    , m_TimeLeft(lifetime)
    , m_pShooter(shooter)
    , m_Targets(std::move(targets))
{
    // Register as observer on every target's HealthComponent
    // so we hear about "PlayerDied" before the memory is freed
    for (auto* target : m_Targets)
        if (auto* health = target->GetComponent<dae::HealthComponent>())
            health->AddObserver(this);
}

tron::BulletComponent::~BulletComponent()
{
    for (auto* target : m_Targets)
        if (auto* health = target->GetComponent<dae::HealthComponent>())
            health->RemoveObserver(this);
}

void tron::BulletComponent::Notify(dae::GameObject* go, unsigned int eventId)
{
    if (eventId != make_sdbm_hash("PlayerDied")) return;

    UnregisterFromTarget(go);
    m_Targets.erase(std::remove(m_Targets.begin(), m_Targets.end(), go), m_Targets.end());
}

void tron::BulletComponent::UnregisterFromTarget(dae::GameObject* target)
{
    if (auto* health = target->GetComponent<dae::HealthComponent>())
        health->RemoveObserver(this);
}

void tron::BulletComponent::CheckTankCollisions()
{
    auto* bulletCollider = GetOwner()->GetComponent<dae::RectColliderComponent>();
    if (!bulletCollider) return;

    const dae::Rect bulletRect = bulletCollider->GetWorldRect();

    for (auto* target : m_Targets)
    {
        // m_Targets only contains live pointers now — no need for IsMarkedForDeath check
        if (target == m_pShooter) continue;

        auto* targetCollider = target->GetComponent<dae::RectColliderComponent>();
        if (!targetCollider) continue;

        if (dae::RectsOverlap(bulletRect, targetCollider->GetWorldRect()))
        {
            if (auto* score = m_pShooter->GetComponent<dae::ScoreComponent>())
                score->GetMegaKill();

            if (auto* health = target->GetComponent<dae::HealthComponent>())
                health->LoseLife();  // fires Notify("PlayerDied") if fatal, cleaning our list

            if (auto* transform = GetOwner()->GetComponent<dae::TransformComponent>())
                tron::SpawnExplosion(*m_pScene, transform->GetLocalPosition(), m_TileSize, m_Scale);

            GetOwner()->MarkForDeath();
            return;
        }
    }
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

    CheckTankCollisions();
}

//void tron::BulletComponent::CheckTankCollisions()
//{
//    auto* bulletCollider = GetOwner()->GetComponent<dae::RectColliderComponent>();
//    if (!bulletCollider) return;
//
//    const dae::Rect bulletRect = bulletCollider->GetWorldRect();
//
//    for (auto* target : m_Targets)
//    {
//        if (!target || target == m_pShooter) continue;
//        if (target->IsMarkedForDeath())      continue;
//
//        auto* targetCollider = target->GetComponent<dae::RectColliderComponent>();
//        if (!targetCollider) continue;
//
//        if (dae::RectsOverlap(bulletRect, targetCollider->GetWorldRect()))
//        {
//            if (auto* health = target->GetComponent<dae::HealthComponent>())
//                health->LoseLife();  // players respawn via observer; enemies die (numLives=1)
//
//            if (auto* transform = GetOwner()->GetComponent<dae::TransformComponent>())
//                tron::SpawnExplosion(*m_pScene, transform->GetLocalPosition(), m_TileSize, m_Scale);
//
//            GetOwner()->MarkForDeath();
//            return; // bullet gone, stop checking
//        }
//    }
//}

bool tron::BulletComponent::CheckWall(GridCollisionComponent* collision,
    float x, float y, float size) const
{
    const float center = size * 0.5f;
    return collision->IsWall(x + center, y + center);
}