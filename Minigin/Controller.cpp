#include "Controller.h"

#ifdef _WIN32
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")
#endif

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
#ifdef _WIN32
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
#endif
		}

		bool IsDownThisFrame(ControllerButton button) const
		{
#ifdef _WIN32
			return (m_ButtonsPressedThisFrame & static_cast<unsigned int>(button)) != 0;
#else
			return false;
#endif
		}

		bool IsUpThisFrame(ControllerButton button) const
		{
#ifdef _WIN32
			return (m_ButtonsReleasedThisFrame & static_cast<unsigned int>(button)) != 0;
#else
			return false;
#endif
		}

		bool IsPressed(ControllerButton button) const
		{
#ifdef _WIN32
			return (m_PreviousState.Gamepad.wButtons & static_cast<unsigned int>(button)) != 0;
#else
			return false;
#endif
		}

	private:
#ifdef _WIN32
		XINPUT_STATE m_PreviousState{};
#endif
		unsigned int m_ButtonsPressedThisFrame{ 0 };
		unsigned int m_ButtonsReleasedThisFrame{ 0 };
		unsigned int m_ControllerIndex;
	};
}