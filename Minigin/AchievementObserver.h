#pragma once
#include "Observer.h"
#include "EventID.h"
#include "Achievements.h"

class CSteamAchievements;
namespace dae
{
	class AchievementObserver final : public Observer
	{
	public:
		void Notify(GameObject* gameObject, unsigned int eventId) override
		{
			switch (eventId)
			{
			default:
				break;
			}
		}

	private:

	};

}