#pragma once
#include "Command.h"
#include <glm/glm.hpp>

namespace tron
{
    class GunComponent;

    class GunShootCommand final : public dae::Command
    {
    public:
        GunShootCommand(GunComponent* gun, const glm::vec2& direction);

        void Execute(float deltaTime) override;

    private:
        GunComponent* m_pGun;
        glm::vec2 m_Direction;
    };
}
