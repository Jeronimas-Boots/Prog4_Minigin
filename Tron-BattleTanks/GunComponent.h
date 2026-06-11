#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae { class RenderComponent; }

namespace tron
{
    class GunComponent final : public dae::Component
    {
    public:
        GunComponent(dae::GameObject* owner, int controllerIndex);
        void Update(float deltaTime) override;

    private:
        int m_ControllerIndex;
        dae::RenderComponent* m_pRenderComponent{ nullptr };
    };
}