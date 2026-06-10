#include "RenderComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Texture2D.h"

dae::RenderComponent::RenderComponent(GameObject* owner, std::shared_ptr<Texture2D> texture)
    : Component(owner)
    , m_pTexture(std::move(texture))
{
}

void dae::RenderComponent::Render() const
{
    if (!m_pTexture) return;
    TransformComponent* transform = GetOwner()->GetComponent<TransformComponent>();
    if (!transform) return;

    const glm::vec3& pos = transform->GetWorldPosition();

    if (m_ScaleX == 1.f && m_ScaleY == 1.f)
    {
        Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
    }
    else
    {
        float texW{}, texH{};
        SDL_GetTextureSize(m_pTexture->GetSDLTexture(), &texW, &texH);
        Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y,
            texW * m_ScaleX, texH * m_ScaleY);
    }
}

void dae::RenderComponent::SetTexture(std::shared_ptr<Texture2D> texture)
{
    m_pTexture = std::move(texture);
}

void dae::RenderComponent::SetScale(float scaleX, float scaleY)
{
    m_ScaleX = scaleX;
    m_ScaleY = scaleY;
}