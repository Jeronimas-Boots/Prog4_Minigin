#include "GunComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "InputManager.h"
#include "BulletBuilder.h"
#include <glm/gtc/constants.hpp>
#include <cmath>

tron::GunComponent::GunComponent(dae::GameObject* owner, int controllerIndex,
    dae::Scene* scene, GridCollisionComponent* collision,
    float tileSize, float scale)
    : Component(owner)
    , m_ControllerIndex(controllerIndex)
    , m_pRenderComponent(owner->GetComponent<dae::RenderComponent>())
    , m_pScene(scene)
    , m_Collision(collision)
    , m_TileSize(tileSize)
    , m_Scale(scale)
{
}

void tron::GunComponent::Update(float deltaTime)
{
    if (!m_pRenderComponent) return;

    if (m_FireCooldown > 0.f)
        m_FireCooldown -= deltaTime;

    // Player-only: aim with right stick, fire on cooldown
    if (m_ControllerIndex < 0)
        return; // enemy guns are driven externally via Shoot()

    const auto& input = dae::InputManager::GetInstance();
    const glm::vec2 stick = input.GetRightStick(m_ControllerIndex);

    if (stick.x == 0.f && stick.y == 0.f)
        return; // inside deadzone — hold last aim, don't fire

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

    tron::SpawnBullet(*m_pScene, m_Collision, spawnPos, direction,
        renderAngle, m_TileSize, m_Scale);

    m_FireCooldown = m_FireRate;
    return true;
}