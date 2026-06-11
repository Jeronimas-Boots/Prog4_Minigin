#include "GunComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "InputManager.h"
#include "Controller.h"
#include "InputManager.h"
#include <glm/gtc/constants.hpp>
#include <cmath>


tron::GunComponent::GunComponent(dae::GameObject* owner, int controllerIndex)
    : Component(owner)
    , m_ControllerIndex(controllerIndex)
    , m_pRenderComponent(owner->GetComponent<dae::RenderComponent>())
{
}

void tron::GunComponent::Update(float /*deltaTime*/)
{
    if (!m_pRenderComponent) return;

    const auto& input = dae::InputManager::GetInstance();
    const glm::vec2 stick = input.GetRightStick(m_ControllerIndex);

    if (stick.x != 0.f || stick.y != 0.f)
    {
        const float angle = std::atan2(-stick.x, -stick.y)
            * (180.f / glm::pi<float>()) + 90.f;
        m_pRenderComponent->SetAngle(angle);
    }
}