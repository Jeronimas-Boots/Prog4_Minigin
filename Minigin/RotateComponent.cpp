#include "RotateComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include <glm/glm.hpp>

dae::RotateComponent::RotateComponent(GameObject* owner, float radius, float speed)
	: Component(owner)
	, m_Radius(radius)
	, m_Speed(speed)
{
	m_Transform = GetOwner()->GetComponent<TransformComponent>();
	if (m_Transform)
	{
		m_CenterPosition = m_Transform->GetLocalPosition();
	}
}

void dae::RotateComponent::Update(float deltaTime)
{
	if (!m_Transform)
		return;

	m_Angle += m_Speed * deltaTime;

	const float x = m_CenterPosition.x + m_Radius * glm::cos(m_Angle);
	const float y = m_CenterPosition.y + m_Radius * glm::sin(m_Angle);

	m_Transform->SetLocalPosition(x, y);
}