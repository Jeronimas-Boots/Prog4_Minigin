#pragma once
#include "GameObjectCommand.h"
#include "GameObject.h"
#include "HealthComponent.h"

namespace dae
{
	class DamageCommand : public GameObjectCommand
	{
	public:
		DamageCommand(GameObject* gameObject)
			: GameObjectCommand(gameObject)
		{

		}
		void Execute(float /*deltaTime*/) override
		{
			if (auto healthComp = GetGameObject()->GetComponent<HealthComponent>())
			{
				healthComp->LoseLife();
			}
		}
	};
}