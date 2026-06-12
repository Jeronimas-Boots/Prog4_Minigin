#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae { class RenderComponent; class Scene; }

namespace tron
{
    class GridCollisionComponent;
    class GunComponent final : public dae::Component
    {
    public:
        GunComponent(dae::GameObject* owner, int controllerIndex, dae::Scene* scene, GridCollisionComponent* collision, float tileSize, float scale);

        void Update(float deltaTime) override;

    private:
        int m_ControllerIndex;
        dae::RenderComponent* m_pRenderComponent{ nullptr };
        dae::Scene* m_pScene;
        GridCollisionComponent* m_Collision;
        float m_TileSize;
        float m_Scale;

        float m_FireCooldown{ 0.f };
        const float m_FireRate = 0.5f;
    };
}