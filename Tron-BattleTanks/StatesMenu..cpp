#include "StatesMenu.h"
#include "MenuStateComponent.h"

void tron::IdleMenuState::OnEnter(MenuStateComponent& menu)
{
	menu.UpdateGameModeText();
}