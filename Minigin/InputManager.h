#pragma once
#include "Singleton.h"
#include "Controller.h"
#include <memory>
#include <map>
#include <SDL3/SDL_scancode.h>

namespace dae
{
	class Command;

	enum class KeyState
	{
		Down,
		Up,
		Pressed
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager();

		bool ProcessInput(float deltaTime);

		// Controller bindings
		void BindCommand(unsigned int controllerIndex, ControllerButton button, KeyState keyState, std::unique_ptr<Command> command);
		void UnbindCommand(unsigned int controllerIndex, ControllerButton button, KeyState keyState);

		// KeyBoard bindings
		void BindCommand(SDL_Scancode key, KeyState keyState, std::unique_ptr<Command> command);
		void UnbindCommand(SDL_Scancode key, KeyState keyState);

	private:
		struct ControllerKey
		{
			unsigned int controllerIndex;
			ControllerButton button;
			KeyState keyState;

			bool operator<(const ControllerKey& other) const
			{
				if (controllerIndex != other.controllerIndex)
					return controllerIndex < other.controllerIndex;
				if (button != other.button)
					return button < other.button;
				return keyState < other.keyState;
			}
		};

		struct KeyboardKey
		{
			SDL_Scancode scancode;
			KeyState keyState;

			bool operator<(const KeyboardKey& other) const
			{
				if (scancode != other.scancode)
					return scancode < other.scancode;
				return keyState < other.keyState;
			}
		};

		static constexpr unsigned int MAX_CONTROLLERS = 4;
		std::unique_ptr<Controller> m_Controllers[MAX_CONTROLLERS];

		std::map<ControllerKey, std::unique_ptr<Command>> m_ControllerCommands;
		std::map<KeyboardKey, std::unique_ptr<Command>> m_KeyboardCommands;

		const bool* m_KeyboardState{ nullptr };
		bool* m_PreviousKeyboardState{ nullptr };
		int m_NumKeys{ 0 };
	};

}
