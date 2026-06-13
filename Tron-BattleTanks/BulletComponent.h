#pragma once
#include "Component.h"
#include "Observer.h"
#include <glm/glm.hpp>
#include <vector>

namespace dae { class Scene; class RenderComponent; class GameObject; }
namespace tron { class GridCollisionComponent; }

namespace tron
{
    class BulletComponent final : public dae::Component, public dae::Observer
    {
    public:
        BulletComponent(dae::GameObject* owner, GridCollisionComponent* collision,
            dae::Scene* scene, const glm::vec2& direction,
            float speed, float tileSize, float scale, float lifetime,
            const dae::GameObject* shooter,
            std::vector<dae::GameObject*> targets);

        ~BulletComponent() override;

        void Update(float deltaTime) override;
        void Notify(dae::GameObject* go, unsigned int eventId) override;

    private:
        GridCollisionComponent* m_Collision;
        dae::Scene* m_pScene;
        dae::RenderComponent* m_pRenderComponent{ nullptr };
        glm::vec2               m_Direction;
        float                   m_Speed;
        float                   m_TileSize;
        float                   m_Scale;
        float                   m_TimeLeft;

        const dae::GameObject* m_pShooter;
        std::vector<dae::GameObject*>   m_Targets;

        bool CheckWall(GridCollisionComponent* collision, float x, float y, float size) const;
        void CheckTankCollisions();
        void UnregisterFromTarget(dae::GameObject* target);
    };
}