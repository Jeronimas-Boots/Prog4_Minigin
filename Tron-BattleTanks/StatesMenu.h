#pragma once
#include "MenuState.h"

namespace tron
{
	class IdleMenuState final : public MenuState
	{
	public:
		void OnEnter(MenuStateComponent& menu) override;
	};
}