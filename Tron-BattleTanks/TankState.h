#pragma once
#include <memory>

namespace tron
{
	class Tank;

	class TankState
	{
	public: 
		virtual ~TankState() = default;
		virtual std::unique_ptr<TankState> HandleInput(Tank& tank) { return nullptr; }
		virtual std::unique_ptr<TankState> Update(Tank& tank) { return nullptr; }

		virtual void OnEnter(Tank& tank) {}
		virtual void OnExit(Tank& tank) {}
	};
}