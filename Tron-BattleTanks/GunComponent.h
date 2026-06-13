#pragma once
#include "Component.h"
#include "Observer.h"
#include <glm/glm.hpp>
#include <vector>

namespace dae { class RenderComponent; class Scene; class GameObject; }
namespace tron { class GridCollisionComponent; }

namespace tron
{
    class GunComponent final : public dae::Component, public dae::Observer
    {
    public:
        GunComponent(dae::GameObject* owner, int controllerIndex, dae::Scene* scene,
            GridCollisionComponent* collision, float tileSize, float scale);
        ~GunComponent() override;

        void Update(float deltaTime) override;
        bool Shoot(const glm::vec2& direction);

        void SetBulletTargets(std::vector<dae::GameObject*> targets);
        void Notify(dae::GameObject* go, unsigned int eventId) override;

    private:
        int m_ControllerIndex;
        dae::RenderComponent* m_pRenderComponent{ nullptr };
        dae::Scene* m_pScene;
        GridCollisionComponent* m_Collision;
        float m_TileSize;
        float m_Scale;
        float m_FireCooldown{ 0.f };
        const float m_FireRate = 0.5f;

        std::vector<dae::GameObject*> m_BulletTargets;
    };
}