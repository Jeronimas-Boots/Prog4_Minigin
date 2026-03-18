#include "HealthComponent.h"
#include "GameObject.h"
#include <algorithm>
#include "EventID.h"

dae::HealthComponent::HealthComponent(GameObject* owner, int numLives)
	: Component(owner)
	, m_Lives(numLives)
{
	//NotifyObservers(make_sdbm_hash("HealthChanged"));
}

void dae::HealthComponent::LoseLife()
{
	--m_Lives;

	NotifyObservers(make_sdbm_hash("HealthChanged"));

	if (m_Lives < 0)
		Die();
}

void dae::HealthComponent::GainLife()
{
	++m_Lives;

	NotifyObservers(make_sdbm_hash("HealthChanged"));
}

void dae::HealthComponent::Die()
{
	NotifyObservers(make_sdbm_hash("PlayerDied"));
	GetOwner()->MarkForDeath();
}

void dae::HealthComponent::AddObserver(Observer* observer)
{
	// check if observer is not already in m_Observers
	if (std::find(m_Observers.begin(), m_Observers.end(), observer) == m_Observers.end())
	{
		m_Observers.push_back(observer);
	}
}

void dae::HealthComponent::RemoveObserver(Observer* observer)
{
	m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end());
}

void dae::HealthComponent::NotifyObservers(unsigned int eventId)
{
	for (auto observer : m_Observers)
	{
		observer->Notify(GetOwner(), eventId);
	}
}
