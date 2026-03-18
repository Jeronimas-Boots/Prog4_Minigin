#include "HealthComponent.h"
#include "GameObject.h"

dae::HealthComponent::HealthComponent(GameObject* owner, int numLives)
	: Component(owner)
	, m_Lives(numLives)
{
}

void dae::HealthComponent::LoseLife()
{
	if (m_Lives == 0)
		Die();
	--m_Lives;
}

void dae::HealthComponent::GainLife()
{
	++m_Lives;
}

void dae::HealthComponent::Die()
{
	//m_Owner->MarkForDeath();
}
