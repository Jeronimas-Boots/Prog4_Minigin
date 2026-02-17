#include "FPSComponent.h"
#include "GameObject.h"
#include "TextComponent.h"

dae::FPSComponent::FPSComponent(GameObject* owner)
	: Component(owner)
{
}

void dae::FPSComponent::Update(float deltaTime)
{
	m_AccumTime += deltaTime;
	++m_FrameCount;
	if (m_AccumTime >= 1.0f)
	{
		int fps = m_FrameCount;
		TextComponent* textComp = dynamic_cast<TextComponent*>(GetOwner()->GetComponent("TextComponent"));
		if (textComp)
		{
			textComp->SetText("FPS: " + std::to_string(fps));
		}
		m_AccumTime -= 1.0f;
		m_FrameCount = 0;
	}
}
