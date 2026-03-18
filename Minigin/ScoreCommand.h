#pragma once
#include "GameObjectCommand.h"
#include "GameObject.h"
#include "ScoreComponent.h"

namespace dae
{
	class ScoreCommand : public GameObjectCommand
	{
	public:
		ScoreCommand(GameObject* gameObject, bool isMegaKill)
			: GameObjectCommand(gameObject)
			, m_IsmegaKill(isMegaKill)
		{

		}
		void Execute(float /*deltaTime*/) override
		{
			if (auto scoreComp = GetGameObject()->GetComponent<ScoreComponent>())
			{
				if (m_IsmegaKill)
					scoreComp->GetKill();
				else 
					scoreComp->GetMegaKill();
			}
		}
	private:
		bool m_IsmegaKill;
	};
}