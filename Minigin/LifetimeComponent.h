#pragma once
#include "Component.h"

namespace dae
{
    class LifetimeComponent final : public Component
    {
    public:
        LifetimeComponent(GameObject* owner, float lifetimeSeconds);
        void Update(float deltaTime) override;

    private:
        float m_TimeLeft;
    };
}