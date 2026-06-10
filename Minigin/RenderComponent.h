#pragma once
#include "Component.h"
#include <memory>
#include "Texture2D.h"

namespace dae
{
	class RenderComponent final : public Component
	{
	public:
		RenderComponent(GameObject* owner, std::shared_ptr<Texture2D> Texture2D);

		void Render() const override;

		void SetTexture(std::shared_ptr<Texture2D> texture);
		void SetScale(float scaleX, float scaleY);
		
	private:
		std::shared_ptr<Texture2D> m_pTexture;
		float m_ScaleX{ 1.f };
		float m_ScaleY{ 1.f };
	};
}