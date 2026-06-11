#pragma once
#include <memory>
#include "GameMode.h"

namespace tron
{
	class MenuStateComponent;

	class MenuState
	{
	public:
		virtual ~MenuState() = default;
		virtual std::unique_ptr<MenuState> Update(MenuStateComponent&)
		{
			return nullptr;
		}
		virtual void OnEnter(MenuStateComponent&) {}
		virtual void OnExit(MenuStateComponent&) {}
	};
}