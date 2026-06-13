#pragma once
#include "Component.h"
#include "Observer.h"
#include <glm/glm.hpp>

namespace dae { class HealthComponent; }

namespace tron
{
    class RespawnComponent final : public dae::Component, public dae::Observer
    {
    public:
        RespawnComponent(dae::GameObject* owner, dae::HealthComponent* health, glm::vec2 spawnPos);
        ~RespawnComponent() override;

        void Notify(dae::GameObject* go, unsigned int eventId) override;

    private:
        dae::HealthComponent* m_pHealth;
        glm::vec2 m_SpawnPos;
    };
}