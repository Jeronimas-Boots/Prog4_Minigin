#pragma once 
#include "TankState.h"
#include <memory>

namespace tron
{
	class Tank
	{
	private:
		std::unique_ptr<TankState> m_pState;

	public:

		Tank(std::unique_ptr<TankState> initialState);
			
		void HandleInput();
		void Update();

		void Wander();
		void Shoot();
		bool IsTargetInRange() const;
	};
}