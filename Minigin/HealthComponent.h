#pragma once
#include "Component.h"

namespace dae
{
	class HealthComponent : public Component
	{
	public:
		HealthComponent(GameObject* owner, int numLives = 3);

		void LoseLife();
		void GainLife();
		void Die();

	private:
		int m_Lives;
	};

}