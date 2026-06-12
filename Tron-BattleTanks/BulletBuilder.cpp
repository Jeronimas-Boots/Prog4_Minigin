#include "BulletBuilder.h"
#include "BulletComponent.h"
#include "LifetimeComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "ResourceManager.h"

namespace
{
    constexpr float kBulletSpeedMultiplier = 20.f;
    constexpr float kBulletLifetime = 2.5f;   
    constexpr float kExplosionLifetime = 0.3f;
}

void tron::SpawnBullet(dae::Scene& scene, GridCollisionComponent* collision, const glm::vec3& position, const glm::vec2& direction, float renderAngle, float tileSize, float scale)
{
    // offset = (48 - 12) / 2 * scale = 18 * scale (based on size of the tank and the bullet png)
    const float centerOffset = 18.f * scale;
    auto bulletGO = std::make_unique<dae::GameObject>();

    bulletGO->AddComponent<dae::TransformComponent>(
        std::make_unique<dae::TransformComponent>(bulletGO.get(), position.x + centerOffset, position.y + centerOffset));

    bulletGO->AddComponent<dae::RenderComponent>(
        std::make_unique<dae::RenderComponent>(
            bulletGO.get(),
            dae::ResourceManager::GetInstance().LoadTexture("BulletNPC.png")));

    auto* render = bulletGO->GetComponent<dae::RenderComponent>();
    render->SetScale(scale, scale);
    render->SetAngle(renderAngle);

    bulletGO->AddComponent<tron::BulletComponent>(
        std::make_unique<tron::BulletComponent>(
            bulletGO.get(), collision, &scene, direction,
            tileSize * kBulletSpeedMultiplier, tileSize, scale, kBulletLifetime));

    scene.Add(std::move(bulletGO));
}

void tron::SpawnExplosion(dae::Scene& scene, const glm::vec3& position, float /*tileSize*/, float scale)
{
    auto explosionGO = std::make_unique<dae::GameObject>();

    explosionGO->AddComponent<dae::TransformComponent>(
        std::make_unique<dae::TransformComponent>(explosionGO.get(), position.x, position.y));

    explosionGO->AddComponent<dae::RenderComponent>(
        std::make_unique<dae::RenderComponent>(
            explosionGO.get(),
            dae::ResourceManager::GetInstance().LoadTexture("BulletExplosion.png")));

    explosionGO->GetComponent<dae::RenderComponent>()->SetScale(scale, scale);

    explosionGO->AddComponent<dae::LifetimeComponent>(
        std::make_unique<dae::LifetimeComponent>(explosionGO.get(), kExplosionLifetime));

    scene.Add(std::move(explosionGO));
}