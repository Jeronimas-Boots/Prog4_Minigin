#pragma once
#include <memory>

namespace tron
{
	class TankStateComponent;

	class TankState
	{
	public: 
		virtual ~TankState() = default;
		virtual std::unique_ptr<TankState> HandleInput(TankStateComponent& tank) { return nullptr; }
		virtual std::unique_ptr<TankState> Update(TankStateComponent& tank) { return nullptr; }

		virtual void OnEnter(TankStateComponent& tank) {}
		virtual void OnExit(TankStateComponent& tank) {}
	};
}