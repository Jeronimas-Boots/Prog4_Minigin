#include "RectColliderComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "RenderComponent.h"

dae::RectColliderComponent::RectColliderComponent(GameObject* owner, float width, float height,
    std::string tag)
    : Component(owner), m_Width(width), m_Height(height), m_Tag(std::move(tag))
{
}

dae::Rect dae::RectColliderComponent::GetWorldRect() const
{
    const auto* transform = GetOwner()->GetComponent<TransformComponent>();
    const glm::vec3 pos = transform ? transform->GetWorldPosition() : glm::vec3{};

    float w = m_Width, h = m_Height;
    if (w == 0.f || h == 0.f)
    {
        if (const auto* render = GetOwner()->GetComponent<RenderComponent>())
        {
            const auto size = render->GetScaledSize();
            if (w == 0.f) w = size.x;
            if (h == 0.f) h = size.y;
        }
    }
    return { pos.x, pos.y, w, h };
}