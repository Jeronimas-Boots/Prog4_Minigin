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

		std::string GetTypeName() const override { return "RenderComponent"; }
	private:
		std::shared_ptr<Texture2D> m_pTexture;
	};
}