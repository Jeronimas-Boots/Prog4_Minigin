#pragma once
#include "Component.h"
#include "Observer.h"

namespace dae
{
	class TextComponent;
	class ScoreComponent;

	class ScoreUIComponent final : public Component, public Observer
	{
	public:
		ScoreUIComponent(GameObject* owner, ScoreComponent* scoreComponent);

		void Notify(GameObject* object, unsigned int eventId) override;

	private:
		TextComponent* m_TextComponent{ nullptr };
		ScoreComponent* m_ScoreComponent{ nullptr };
	};
}