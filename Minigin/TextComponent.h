#pragma once
#include "Component.h"
#include <string>
#include <memory>
#include <SDL3_ttf/SDL_ttf.h>

namespace dae
{
	class Font;
	class Texture2D;
	class RenderComponent;
	class TextComponent final : public Component
	{
	public:
		TextComponent(GameObject* owner, const std::string& text, const SDL_Color& color, std::shared_ptr<Font> font);

		void Update(float deltaTime) override;
		void Render() const override;

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);

	private:
		bool m_NeedsUpdate{ true };
		std::string m_Text;
		SDL_Color m_Color{ 255, 255, 255, 255 };
		
		std::shared_ptr<Font> m_pFont;
		std::shared_ptr<Texture2D> m_ptextTexture;
	};
}