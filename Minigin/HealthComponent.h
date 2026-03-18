#pragma once
#include "Component.h"
#include "Observer.h"
#include <vector>

namespace dae
{
	class HealthComponent final : public Component
	{
	public:
		HealthComponent(GameObject* owner, int numLives = 3);

		void LoseLife();
		void GainLife();
		void Die();

		int GetLives() const { return m_Lives; };

		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);

	protected:
		void NotifyObservers(unsigned int eventId);
	private:
		int m_Lives;
		std::vector<Observer*> m_Observers;
	};
}