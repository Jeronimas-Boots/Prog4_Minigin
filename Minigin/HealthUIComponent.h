#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae
{
	class TextComponent;
	class HealthComponent;

	class HealthUIComponent final : public Component, public Observer
	{
	public:
		HealthUIComponent(GameObject* owner, HealthComponent* healthcomponent);

		void Notify(GameObject* object, unsigned int eventId) override;

	private:
		TextComponent* m_TextComponent{ nullptr };
		HealthComponent* m_HealthComponent{ nullptr };
	};
}