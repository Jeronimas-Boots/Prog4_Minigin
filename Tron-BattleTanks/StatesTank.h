#pragma once
#include "TankState.h"
#include "Tank.h"

namespace tron
{
	class TankShootState : public TankState
	{
	public:
		std::unique_ptr<TankState> Update(Tank& tank) override;
	};

	class TankWanderState : public TankState
	{
		std::unique_ptr<TankState> Update(Tank& tank) override;
	};
}