#pragma once
#include "TankState.h"
#include "TankStateComponent.h"

namespace tron
{
	class TankShootState : public TankState
	{
	public:
		void OnEnter(TankStateComponent& stateComponent) override;
		std::unique_ptr<TankState> Update(TankStateComponent& tank) override;
	};

	class TankWanderState : public TankState
	{
	public:
		void OnEnter(TankStateComponent& stateComponent) override;
		std::unique_ptr<TankState> Update(TankStateComponent& tank) override;
	};
}