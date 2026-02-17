#include "TextComponent.h"
#include "Font.h"
#include "Renderer.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Texture2D.h"

#include <stdexcept>

dae::TextComponent::TextComponent(GameObject* owner, const std::string& text, const SDL_Color& color, std::shared_ptr<Font> font)
	: Component(owner)
	, m_Text(text)
	, m_Color(color)
	, m_pFont(std::move(font))
{
}

void dae::TextComponent::Update(float /*deltaTime*/)
{
	if (m_NeedsUpdate)
	{

		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_Text.length(), m_Color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			SDL_DestroySurface(surf);
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_DestroySurface(surf);
		m_ptextTexture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void dae::TextComponent::Render() const
{
	if (!m_ptextTexture) return;

	TransformComponent* transform = dynamic_cast<TransformComponent*>(GetOwner()->GetComponent("TransformComponent"));
	if (!transform) return;

	const glm::vec3& pos = transform->GetPositioin();
	Renderer::GetInstance().RenderTexture(*m_ptextTexture, pos.x, pos.y);
}

void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void dae::TextComponent::SetColor(const SDL_Color& color)
{
	m_Color = color;
	m_NeedsUpdate = true;
}