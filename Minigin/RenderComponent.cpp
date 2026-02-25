#include "RenderComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Renderer.h"

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
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}

void dae::RenderComponent::SetTexture(std::shared_ptr<Texture2D> texture)
{
	m_pTexture = std::move(texture);
}