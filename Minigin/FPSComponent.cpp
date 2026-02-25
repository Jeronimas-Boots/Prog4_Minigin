#include "FPSComponent.h"
#include "GameObject.h"
#include <sstream>
#include <iomanip>

dae::FPSComponent::FPSComponent(GameObject* owner)
	: Component(owner)
{
}

void dae::FPSComponent::Update(float deltaTime)
{
	m_AccumTime += deltaTime;
	++m_FrameCount;
	if (m_AccumTime >= 1.f)
	{
		if (!m_TextComponent)
		{
			m_TextComponent = GetOwner()->GetComponent<TextComponent>();
		}

		if (m_TextComponent)
		{
			float fps = m_FrameCount / m_AccumTime;

			std::ostringstream oss;
			oss << std::fixed << std::setprecision(1) << fps;
			m_TextComponent->SetText("FPS: " + oss.str());
		}

		m_AccumTime -= 1.0f;
		m_FrameCount = 0;
	}
}
