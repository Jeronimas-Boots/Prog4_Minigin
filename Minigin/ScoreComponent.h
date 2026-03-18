#pragma once
#include "Component.h"
#include "Observer.h"
#include <vector>

namespace dae
{
	class ScoreComponent final : public Component
	{
	public:
		ScoreComponent(GameObject* owner);

		// Not sure yet how you gain points in the tron game... haven't played it yet
		void GetKill();
		void GetMegaKill();

		int GetScore() const { return m_Score; };

		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);

	protected:
		void NotifyObservers(unsigned int eventId);
	private:
		int m_Score;
		std::vector<Observer*> m_Observers;
	};
}