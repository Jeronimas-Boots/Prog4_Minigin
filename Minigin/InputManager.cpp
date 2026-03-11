#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include "InputManager.h"
#include "Command.h"

dae::InputManager::InputManager()
{
	// Initialize controllers
	for (unsigned int i = 0; i < MAX_CONTROLLERS; ++i)
	{
		m_Controllers[i] = std::make_unique<Controller>(i);
	}

	// Initialize keyboard state tracking
	m_KeyboardState = SDL_GetKeyboardState(&m_NumKeys);
	m_PreviousKeyboardState = new bool[m_NumKeys];
	memcpy(m_PreviousKeyboardState, m_KeyboardState, m_NumKeys);
}

dae::InputManager::~InputManager()
{
	delete[] m_PreviousKeyboardState;
}

bool dae::InputManager::ProcessInput()
{
	// Update controllers
	for (auto& controller : m_Controllers)
	{
		controller->Update();
	}

	// Process controller commands
	for (const auto& [key, command] : m_ControllerCommands)
	{
		const auto& controller = m_Controllers[key.controllerIndex];

		bool shouldExecute = false;
		switch (key.keyState)
		{
		case KeyState::Down:
			shouldExecute = controller->IsDownThisFrame(key.button);
			break;
		case KeyState::Up:
			shouldExecute = controller->IsUpThisFrame(key.button);
			break;
		case KeyState::Pressed:
			shouldExecute = controller->IsPressed(key.button);
			break;
		}

		if (shouldExecute && command)
		{
			command->Execute();
		}
	}

	// Process keyboard commands
	for (const auto& [key, command] : m_KeyboardCommands)
	{
		bool shouldExecute = false;
		const bool isCurrentlyPressed = m_KeyboardState[key.scancode];
		const bool wasPreviouslyPressed = m_PreviousKeyboardState[key.scancode];

		switch (key.keyState)
		{
		case KeyState::Down:
			shouldExecute = isCurrentlyPressed && !wasPreviouslyPressed;
			break;
		case KeyState::Up:
			shouldExecute = !isCurrentlyPressed && wasPreviouslyPressed;
			break;
		case KeyState::Pressed:
			shouldExecute = isCurrentlyPressed;
			break;
		}

		if (shouldExecute && command)
		{
			command->Execute();
		}
	}

	// Store current keyboard state for next frame
	memcpy(m_PreviousKeyboardState, m_KeyboardState, m_NumKeys);

	// Handle SDL events
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_EVENT_QUIT) {
			return false;
		}

		//process event for IMGUI
		ImGui_ImplSDL3_ProcessEvent(&e);
	}

	return true;
}

void dae::InputManager::BindCommand(unsigned int controllerIndex, ControllerButton button, KeyState keyState, std::unique_ptr<Command> command)
{
	if (controllerIndex >= MAX_CONTROLLERS)
		return;

	ControllerKey key{ controllerIndex, button, keyState };
	m_ControllerCommands[key] = std::move(command);
}

void dae::InputManager::UnbindCommand(unsigned int controllerIndex, ControllerButton button, KeyState keyState)
{
	if (controllerIndex >= MAX_CONTROLLERS)
		return;

	ControllerKey key{ controllerIndex, button, keyState };
	m_ControllerCommands.erase(key);
}

void dae::InputManager::BindCommand(SDL_Scancode scancode, KeyState keyState, std::unique_ptr<Command> command)
{
	KeyboardKey key{ scancode, keyState };
	m_KeyboardCommands[key] = std::move(command);
}

void dae::InputManager::UnbindCommand(SDL_Scancode scancode, KeyState keyState)
{
	KeyboardKey key{ scancode, keyState };
	m_KeyboardCommands.erase(key);
}
