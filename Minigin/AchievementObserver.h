#pragma once
#include "Component.h"
#include "Observer.h"

class CSteamAchievements;
namespace dae
{
	class ScoreComponent;
	class GameObject;

	class AchievementObserver final : public Component, public Observer
	{
	public:
		AchievementObserver(GameObject* owner, CSteamAchievements* pSteamAchievements);

		void Notify(GameObject* gameObject, unsigned int eventId) override;

	private:
		CSteamAchievements* m_SteamAchievements;
		ScoreComponent* m_ScoreComp;
	};

}