#include "Controller.h"

#ifdef _WIN32
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")
#else
#include <SDL.h>
#endif

namespace dae
{
	class Controller::ControllerImpl
	{
	public:
		explicit ControllerImpl(unsigned int controllerIndex)
			: m_ControllerIndex(controllerIndex)
		{
#ifndef _WIN32
			// Open SDL gamepad by index
			m_Gamepad = SDL_OpenGamepad(SDL_GetGamepads(nullptr)[controllerIndex]);
#endif
		}

		~ControllerImpl()
		{
#ifndef _WIN32
			if (m_Gamepad)
			{
				SDL_CloseGamepad(m_Gamepad);
			}
#endif
		}

		void Update()
		{
			m_ButtonsPressedThisFrame = 0;
			m_ButtonsReleasedThisFrame = 0;

#ifdef _WIN32
			// XInput implementation (Windows)
			XINPUT_STATE currentState{};
			if (XInputGetState(m_ControllerIndex, &currentState) == ERROR_SUCCESS)
			{
				const auto buttonChanges = m_PreviousState.Gamepad.wButtons ^ currentState.Gamepad.wButtons;
				m_ButtonsPressedThisFrame = buttonChanges & currentState.Gamepad.wButtons;
				m_ButtonsReleasedThisFrame = buttonChanges & (~currentState.Gamepad.wButtons);

				m_PreviousState = currentState;
			}
#else
			// SDL implementation (Emscripten/other platforms) I have enlisted the help from LLM's here for I did not know how to make the emscripten work with gamepad
			if (m_Gamepad)
			{
				unsigned int currentState = 0;

				// Map SDL buttons to our button bits
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP)) currentState |= static_cast<unsigned int>(ControllerButton::DPadUp);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN)) currentState |= static_cast<unsigned int>(ControllerButton::DPadDown);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT)) currentState |= static_cast<unsigned int>(ControllerButton::DPadLeft);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT)) currentState |= static_cast<unsigned int>(ControllerButton::DPadRight);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_START)) currentState |= static_cast<unsigned int>(ControllerButton::Start);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_BACK)) currentState |= static_cast<unsigned int>(ControllerButton::Back);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_LEFT_STICK)) currentState |= static_cast<unsigned int>(ControllerButton::LeftThumb);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_RIGHT_STICK)) currentState |= static_cast<unsigned int>(ControllerButton::RightThumb);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER)) currentState |= static_cast<unsigned int>(ControllerButton::LeftShoulder);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER)) currentState |= static_cast<unsigned int>(ControllerButton::RightShoulder);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_SOUTH)) currentState |= static_cast<unsigned int>(ControllerButton::ButtonA);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_EAST)) currentState |= static_cast<unsigned int>(ControllerButton::ButtonB);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_WEST)) currentState |= static_cast<unsigned int>(ControllerButton::ButtonX);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_NORTH)) currentState |= static_cast<unsigned int>(ControllerButton::ButtonY);

				const auto buttonChanges = m_PreviousButtonState ^ currentState;
				m_ButtonsPressedThisFrame = buttonChanges & currentState;
				m_ButtonsReleasedThisFrame = buttonChanges & (~currentState);

				m_PreviousButtonState = currentState;
			}
#endif
		}

		bool IsDownThisFrame(ControllerButton button) const
		{
			return (m_ButtonsPressedThisFrame & static_cast<unsigned int>(button)) != 0;
		}

		bool IsUpThisFrame(ControllerButton button) const
		{
			return (m_ButtonsReleasedThisFrame & static_cast<unsigned int>(button)) != 0;
		}

		bool IsPressed(ControllerButton button) const
		{
#ifdef _WIN32
			return (m_PreviousState.Gamepad.wButtons & static_cast<unsigned int>(button)) != 0;
#else
			return (m_PreviousButtonState & static_cast<unsigned int>(button)) != 0;
#endif
		}

	private:
#ifdef _WIN32
		XINPUT_STATE m_PreviousState{};
#else
		SDL_Gamepad* m_Gamepad{ nullptr };
		unsigned int m_PreviousButtonState{ 0 };
#endif
		unsigned int m_ButtonsPressedThisFrame{ 0 };
		unsigned int m_ButtonsReleasedThisFrame{ 0 };
		unsigned int m_ControllerIndex;
	};

	Controller::Controller(unsigned int controllerIndex)
		: m_Impl(std::make_unique<ControllerImpl>(controllerIndex))
	{
	}

	Controller::~Controller() = default;

	void Controller::Update()
	{
		m_Impl->Update();
	}

	bool Controller::IsDownThisFrame(ControllerButton button) const
	{
		return m_Impl->IsDownThisFrame(button);
	}

	bool Controller::IsUpThisFrame(ControllerButton button) const
	{
		return m_Impl->IsUpThisFrame(button);
	}

	bool Controller::IsPressed(ControllerButton button) const
	{
		return m_Impl->IsPressed(button);
	}
}