#include "HealthUIComponent.h"
#include "HealthComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "EventID.h"

dae::HealthUIComponent::HealthUIComponent(GameObject* owner, HealthComponent* healthcomponent)
	: Component(owner)
	, m_HealthComponent(healthcomponent)
{
	if (m_HealthComponent)
	{
		m_HealthComponent->AddObserver(this);
		Notify(owner, make_sdbm_hash("HealthChanged"));
	}
}

void dae::HealthUIComponent::Notify(GameObject* object, unsigned int eventId)
{
	switch (eventId)
	{
	case make_sdbm_hash("HealthChanged"):
		if (!m_TextComponent)
			m_TextComponent = object->GetComponent<dae::TextComponent>();

		if (m_TextComponent && m_HealthComponent)
			m_TextComponent->SetText("Lives: " + std::to_string(m_HealthComponent->GetLives()));

		break;
	case make_sdbm_hash("PlayerDied"):
		if (m_TextComponent)
			m_TextComponent->SetText("Player Died!");
		break;
	default:
		break;
	}
}
