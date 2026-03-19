#include "AchievementObserver.h"
#include "ScoreComponent.h"
#include "GameObject.h"
#include "Achievements.h"
#include "EventID.h"

dae::AchievementObserver::AchievementObserver(GameObject* owner, CSteamAchievements* pSteamAchievements)
	: Component(owner)
	, m_SteamAchievements(pSteamAchievements)
{
	m_ScoreComp = GetOwner()->GetComponent<dae::ScoreComponent>();
	if (m_ScoreComp)
	{
		m_ScoreComp->AddObserver(this);
	}
}

void dae::AchievementObserver::Notify(GameObject* /*gameObject*/, unsigned int eventId)
{
	if (eventId == make_sdbm_hash("ScoreChanged"))
	{
		if (m_ScoreComp && m_ScoreComp->GetScore() >= 500)
		{
			if (m_SteamAchievements)
				m_SteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");
		}
	}
}
