#include "Controller.h"

#ifdef _WIN32
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")
#else
#include <SDL3/SDL.h>
#endif

#include <glm/glm.hpp>
#include <cmath>
#include <iostream>

namespace dae
{
	class Controller::ControllerImpl
	{
	public:
		explicit ControllerImpl(unsigned int controllerIndex)
			: m_ControllerIndex(controllerIndex)
		{
#ifndef _WIN32
			TryOpenGamepad();
#endif
		}

		~ControllerImpl()
		{
#ifndef _WIN32
			if (m_Gamepad)
				SDL_CloseGamepad(m_Gamepad);
#endif
		}

		void Update()
		{
			m_ButtonsPressedThisFrame = 0;
			m_ButtonsReleasedThisFrame = 0;

#ifdef _WIN32
			XINPUT_STATE currentState{};
			if (XInputGetState(m_ControllerIndex, &currentState) == ERROR_SUCCESS)
			{
				const auto buttonChanges = m_PreviousState.Gamepad.wButtons ^ currentState.Gamepad.wButtons;
				m_ButtonsPressedThisFrame = buttonChanges & currentState.Gamepad.wButtons;
				m_ButtonsReleasedThisFrame = buttonChanges & (~currentState.Gamepad.wButtons);
				m_PreviousState = currentState;
			}
#else
			// Compute this-frame changes from event-driven state
			const auto buttonChanges = m_PreviousButtonState ^ m_CurrentButtonState;
			m_ButtonsPressedThisFrame = buttonChanges & m_CurrentButtonState;
			m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentButtonState);
			m_PreviousButtonState = m_CurrentButtonState;
#endif
		}

#ifndef _WIN32
		void HandleEvent(const SDL_Event& e)
		{
			if (e.type == SDL_EVENT_GAMEPAD_ADDED)
			{
				TryOpenGamepad();
				return;
			}

			if (e.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN ||
				e.type == SDL_EVENT_GAMEPAD_BUTTON_UP)
			{
				if (!m_Gamepad) return;
				if (e.gbutton.which != SDL_GetGamepadID(m_Gamepad)) return;

				unsigned int buttonBit = 0;
				switch (e.gbutton.button)
				{
				case SDL_GAMEPAD_BUTTON_DPAD_UP:        buttonBit = static_cast<unsigned int>(ControllerButton::DPadUp);        break;
				case SDL_GAMEPAD_BUTTON_DPAD_DOWN:      buttonBit = static_cast<unsigned int>(ControllerButton::DPadDown);      break;
				case SDL_GAMEPAD_BUTTON_DPAD_LEFT:      buttonBit = static_cast<unsigned int>(ControllerButton::DPadLeft);      break;
				case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:     buttonBit = static_cast<unsigned int>(ControllerButton::DPadRight);     break;
				case SDL_GAMEPAD_BUTTON_START:          buttonBit = static_cast<unsigned int>(ControllerButton::Start);         break;
				case SDL_GAMEPAD_BUTTON_BACK:           buttonBit = static_cast<unsigned int>(ControllerButton::Back);          break;
				case SDL_GAMEPAD_BUTTON_LEFT_STICK:     buttonBit = static_cast<unsigned int>(ControllerButton::LeftThumb);     break;
				case SDL_GAMEPAD_BUTTON_RIGHT_STICK:    buttonBit = static_cast<unsigned int>(ControllerButton::RightThumb);    break;
				case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER:  buttonBit = static_cast<unsigned int>(ControllerButton::LeftShoulder);  break;
				case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER: buttonBit = static_cast<unsigned int>(ControllerButton::RightShoulder); break;
				case SDL_GAMEPAD_BUTTON_SOUTH:          buttonBit = static_cast<unsigned int>(ControllerButton::ButtonA);       break;
				case SDL_GAMEPAD_BUTTON_EAST:           buttonBit = static_cast<unsigned int>(ControllerButton::ButtonB);       break;
				case SDL_GAMEPAD_BUTTON_WEST:           buttonBit = static_cast<unsigned int>(ControllerButton::ButtonX);       break;
				case SDL_GAMEPAD_BUTTON_NORTH:          buttonBit = static_cast<unsigned int>(ControllerButton::ButtonY);       break;
				default: return;
				}

				if (e.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN)
					m_CurrentButtonState |= buttonBit;
				else
					m_CurrentButtonState &= ~buttonBit;
			}
		}
#endif

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
			return (m_CurrentButtonState & static_cast<unsigned int>(button)) != 0;
#endif
		}

		glm::vec2 GetRightStick() const
		{
			constexpr float deadzone = 0.2f;
			constexpr float maxValue = 32767.f;

#ifdef _WIN32
			float x = m_PreviousState.Gamepad.sThumbRX / maxValue;
			float y = m_PreviousState.Gamepad.sThumbRY / maxValue;
#else
			float x = 0.f, y = 0.f;
			if (m_Gamepad)
			{
				x = SDL_GetGamepadAxis(m_Gamepad, SDL_GAMEPAD_AXIS_RIGHTX) / maxValue;
				y = -SDL_GetGamepadAxis(m_Gamepad, SDL_GAMEPAD_AXIS_RIGHTY) / maxValue;
			}
#endif
			if (std::abs(x) < deadzone && std::abs(y) < deadzone)
				return { 0.f, 0.f };
			return { x, y };
		}

	private:
		unsigned int m_ControllerIndex{ 0 };
		unsigned int m_ButtonsPressedThisFrame{ 0 };
		unsigned int m_ButtonsReleasedThisFrame{ 0 };

#ifdef _WIN32
		XINPUT_STATE m_PreviousState{};
#else
		SDL_Gamepad* m_Gamepad{ nullptr };
		unsigned int m_PreviousButtonState{ 0 };
		unsigned int m_CurrentButtonState{ 0 };

		void TryOpenGamepad()
		{
			if (m_Gamepad) return;

			int count = 0;
			SDL_JoystickID* joysticks = SDL_GetJoysticks(&count);

			std::cout << "Controller " << m_ControllerIndex
				<< ": " << count << " joystick(s) found\n";

			if (joysticks && static_cast<int>(m_ControllerIndex) < count)
			{
				if (SDL_IsGamepad(joysticks[m_ControllerIndex]))
				{
					m_Gamepad = SDL_OpenGamepad(joysticks[m_ControllerIndex]);
					std::cout << "Controller " << m_ControllerIndex
						<< " opened: " << (m_Gamepad ? "success" : SDL_GetError()) << "\n";
				}
				else
				{
					std::cout << "Joystick " << m_ControllerIndex
						<< " is not a gamepad\n";
				}
			}
			SDL_free(joysticks);
		}
#endif
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

	glm::vec2 Controller::GetRightStick() const
	{
		return m_Impl->GetRightStick();
	}

#ifndef _WIN32
	void Controller::HandleEvent(const SDL_Event& e)
	{
		m_Impl->HandleEvent(e);
	}
#endif
}