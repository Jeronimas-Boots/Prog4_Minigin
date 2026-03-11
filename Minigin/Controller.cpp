#include "Controller.h"
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

namespace dae
{
	class Controller::ControllerImpl
	{
	public:
		explicit ControllerImpl(unsigned int controllerIndex)
			: m_ControllerIndex(controllerIndex)
		{
		}

		void Update()
		{
			m_ButtonsPressedThisFrame = 0;
			m_ButtonsReleasedThisFrame = 0;

			XINPUT_STATE currentState{};
			if (XInputGetState(m_ControllerIndex, &currentState) == ERROR_SUCCESS)
			{
				const auto buttonChanges = m_PreviousState.Gamepad.wButtons ^ currentState.Gamepad.wButtons;
				m_ButtonsPressedThisFrame = buttonChanges & currentState.Gamepad.wButtons;
				m_ButtonsReleasedThisFrame = buttonChanges & (~currentState.Gamepad.wButtons);

				m_PreviousState = currentState;
			}
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
			return (m_PreviousState.Gamepad.wButtons & static_cast<unsigned int>(button)) != 0;
		}

	private:
		XINPUT_STATE m_PreviousState{};
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