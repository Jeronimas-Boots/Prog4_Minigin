#pragma once
#include "Component.h"
#include "TextComponent.h"

namespace dae
{
	class FPSComponent final : public Component
	{
	public:
		FPSComponent(GameObject* owner);
		void Update(float deltaTime) override;

		std::string GetTypeName() const override { return "FPSComponent"; }
	private:
		float m_AccumTime{ 0.f };
		int m_FrameCount{ 0 };
		TextComponent* m_TextComponent = nullptr;
	};
}