#pragma once
#include "TankState.h"
#include "TankStateComponent.h"
#include <glm/glm.hpp>

namespace tron
{
	class TankShootState : public TankState
	{
	public:
		void OnEnter(TankStateComponent& stateComponent) override;
		std::unique_ptr<TankState> Update(TankStateComponent& tank, float deltaTime) override;
	};

	class TankWanderState : public TankState
	{
	public:
		void OnEnter(TankStateComponent& stateComponent) override;
		std::unique_ptr<TankState> Update(TankStateComponent& tank, float deltaTime) override;

	private:
		glm::vec2 m_WanderDirection{ 1.f, 0.f };
		float m_DirectionChangeTimer = 0.f;
		float m_DirectionChangeInterval = 1.5f;
	};
}