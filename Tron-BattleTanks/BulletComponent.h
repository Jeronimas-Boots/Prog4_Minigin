#pragma once
#include "Component.h"
#include <glm/glm.hpp>


namespace dae { class Scene; class RenderComponent; }

namespace tron
{
    class GridCollisionComponent;

    class BulletComponent final : public dae::Component
    {
    public:
        BulletComponent(dae::GameObject* owner, GridCollisionComponent* collision, dae::Scene* scene, const glm::vec2& direction, float speed, float tileSize, float scale, float lifetime);

        void Update(float deltaTime) override;

    private:
        GridCollisionComponent* m_Collision;
        dae::Scene* m_pScene;
        dae::RenderComponent* m_pRenderComponent{ nullptr };
        glm::vec2 m_Direction;
        float m_Speed;
        float m_TileSize;
        float m_Scale;
        float m_TimeLeft;


        bool CheckWall(tron::GridCollisionComponent* collision, float x, float y, float size) const;
    };
}