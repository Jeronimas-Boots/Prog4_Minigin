#include "GunComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "InputManager.h"
#include "BulletBuilder.h"
#include "HealthComponent.h"
#include "EventID.h"
#include <glm/gtc/constants.hpp>
#include <cmath>
#include <algorithm>

tron::GunComponent::GunComponent(dae::GameObject* owner, int controllerIndex,
    dae::Scene* scene, GridCollisionComponent* collision, float tileSize, float scale)
    : Component(owner)
    , m_ControllerIndex(controllerIndex)
    , m_pRenderComponent(owner->GetComponent<dae::RenderComponent>())
    , m_pScene(scene)
    , m_Collision(collision)
    , m_TileSize(tileSize)
    , m_Scale(scale)
{
}

tron::GunComponent::~GunComponent()
{
    // Unregister from all targets that are still alive
    for (auto* target : m_BulletTargets)
        if (auto* health = target->GetComponent<dae::HealthComponent>())
            health->RemoveObserver(this);
}

void tron::GunComponent::SetBulletTargets(std::vector<dae::GameObject*> targets)
{
    // Unregister from old targets
    for (auto* target : m_BulletTargets)
        if (auto* health = target->GetComponent<dae::HealthComponent>())
            health->RemoveObserver(this);

    m_BulletTargets = std::move(targets);

    // Register on new targets so we hear when they die
    for (auto* target : m_BulletTargets)
        if (auto* health = target->GetComponent<dae::HealthComponent>())
            health->AddObserver(this);
}

void tron::GunComponent::Notify(dae::GameObject* go, unsigned int eventId)
{
    if (eventId != make_sdbm_hash("PlayerDied")) return;

    // Target is dying — unregister and remove from our list before memory is freed
    if (auto* health = go->GetComponent<dae::HealthComponent>())
        health->RemoveObserver(this);

    m_BulletTargets.erase(
        std::remove(m_BulletTargets.begin(), m_BulletTargets.end(), go),
        m_BulletTargets.end());
}

// Update and Shoot remain unchanged
void tron::GunComponent::Update(float deltaTime)
{
    if (!m_pRenderComponent) return;

    if (m_FireCooldown > 0.f)
        m_FireCooldown -= deltaTime;

    if (m_ControllerIndex < 0)
        return;

    const auto& input = dae::InputManager::GetInstance();
    const glm::vec2 stick = input.GetRightStick(m_ControllerIndex);

    if (stick.x == 0.f && stick.y == 0.f)
        return;

    const float renderAngle = std::atan2(-stick.x, -stick.y)
        * (180.f / glm::pi<float>()) + 90.f;
    m_pRenderComponent->SetAngle(renderAngle);

    if (m_FireCooldown <= 0.f)
    {
        const float rad = renderAngle * (glm::pi<float>() / 180.f);
        const glm::vec2 aimDirection{ std::cos(rad), std::sin(rad) };
        Shoot(aimDirection);
    }
}

bool tron::GunComponent::Shoot(const glm::vec2& direction)
{
    if (m_FireCooldown > 0.f) return false;
    if (direction.x == 0.f && direction.y == 0.f) return false;

    const float renderAngle = std::atan2(direction.y, direction.x) * (180.f / glm::pi<float>());
    if (m_pRenderComponent)
        m_pRenderComponent->SetAngle(renderAngle);

    const glm::vec3 spawnPos = GetOwner()->GetWorldPosition();
    const dae::GameObject* shooter = GetOwner()->GetParent();

    tron::SpawnBullet(*m_pScene, m_Collision, spawnPos, direction,
        renderAngle, m_TileSize, m_Scale, shooter, m_BulletTargets);

    m_FireCooldown = m_FireRate;
    return true;
}