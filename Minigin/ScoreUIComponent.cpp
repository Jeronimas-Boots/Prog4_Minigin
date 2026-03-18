#include "ScoreUIComponent.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "EventID.h"
dae::ScoreUIComponent::ScoreUIComponent(GameObject* owner, ScoreComponent* scoreComponent)
	: Component(owner)
	, m_ScoreComponent(scoreComponent)
{
	if (m_ScoreComponent)
	{
		m_ScoreComponent->AddObserver(this);
		Notify(owner, make_sdbm_hash("ScoreChanged"));
	}
}

dae::ScoreUIComponent::~ScoreUIComponent()
{
	//if (m_ScoreComponent)
	//	m_ScoreComponent->RemoveObserver(this);
}

void dae::ScoreUIComponent::Notify(GameObject* object, unsigned int eventId)
{
	if (eventId == make_sdbm_hash("ScoreChanged"))
	{
		if (!m_TextComponent)
			m_TextComponent = object->GetComponent<dae::TextComponent>();

		if (m_TextComponent && m_ScoreComponent)
			m_TextComponent->SetText("Score: " + std::to_string(m_ScoreComponent->GetScore()));
	}
}
