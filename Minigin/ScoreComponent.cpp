#include "ScoreComponent.h"

dae::ScoreComponent::ScoreComponent(GameObject* owner)
	: Component(owner)
	, m_Score(0)
{
}

void dae::ScoreComponent::GetKill()
{
	m_Score += 10;
}

void dae::ScoreComponent::GetMegaKill()
{
	m_Score += 100;
}

void dae::ScoreComponent::AddObserver(Observer* observer)
{
	if (std::find(m_Observers.begin(), m_Observers.end(), observer) == m_Observers.end())
	{
		m_Observers.push_back(observer);
	}
}

void dae::ScoreComponent::RemoveObserver(Observer* observer)
{
	m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end());
}

void dae::ScoreComponent::NotifyObservers(unsigned int eventId)
{
	for (auto observer : m_Observers)
	{
		observer->Notify(GetOwner(), eventId);
	}
}
