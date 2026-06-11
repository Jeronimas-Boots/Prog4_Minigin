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

namespace dae
{
	class Controller::ControllerImpl
	{
	public:
		explicit ControllerImpl(unsigned int controllerIndex)
#ifdef _WIN32
			: m_ControllerIndex(controllerIndex)
#else
			: m_ControllerIndex(controllerIndex)
#endif
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
			TryOpenGamepad(); // retry every frame until connected

			if (m_Gamepad)
			{
				unsigned int currentState = 0;

				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP))        currentState |= static_cast<unsigned int>(ControllerButton::DPadUp);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN))      currentState |= static_cast<unsigned int>(ControllerButton::DPadDown);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT))      currentState |= static_cast<unsigned int>(ControllerButton::DPadLeft);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT))     currentState |= static_cast<unsigned int>(ControllerButton::DPadRight);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_START))          currentState |= static_cast<unsigned int>(ControllerButton::Start);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_BACK))           currentState |= static_cast<unsigned int>(ControllerButton::Back);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_LEFT_STICK))     currentState |= static_cast<unsigned int>(ControllerButton::LeftThumb);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_RIGHT_STICK))    currentState |= static_cast<unsigned int>(ControllerButton::RightThumb);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER))  currentState |= static_cast<unsigned int>(ControllerButton::LeftShoulder);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER)) currentState |= static_cast<unsigned int>(ControllerButton::RightShoulder);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_SOUTH))          currentState |= static_cast<unsigned int>(ControllerButton::ButtonA);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_EAST))           currentState |= static_cast<unsigned int>(ControllerButton::ButtonB);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_WEST))           currentState |= static_cast<unsigned int>(ControllerButton::ButtonX);
				if (SDL_GetGamepadButton(m_Gamepad, SDL_GAMEPAD_BUTTON_NORTH))          currentState |= static_cast<unsigned int>(ControllerButton::ButtonY);

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
#ifdef _WIN32
		XINPUT_STATE m_PreviousState{};
		unsigned int m_ControllerIndex;
#else
		SDL_Gamepad* m_Gamepad{ nullptr };
		unsigned int m_PreviousButtonState{ 0 };
		unsigned int m_ControllerIndex{ 0 };

		void TryOpenGamepad()
		{
			if (m_Gamepad) return;

			int count = 0;
			SDL_JoystickID* joysticks = SDL_GetJoysticks(&count);
			if (joysticks && static_cast<int>(m_ControllerIndex) < count)
				m_Gamepad = SDL_OpenGamepad(joysticks[m_ControllerIndex]);
			SDL_free(joysticks);
		}
#endif
		unsigned int m_ButtonsPressedThisFrame{ 0 };
		unsigned int m_ButtonsReleasedThisFrame{ 0 };
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
}