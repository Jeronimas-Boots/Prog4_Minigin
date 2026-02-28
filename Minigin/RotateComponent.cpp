#include "RotateComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

dae::RotateComponent::RotateComponent(GameObject* owner, float radius, float speed)
	: Component(owner)
	, m_Radius(radius)
	, m_Speed(speed)
{
	m_Transform = GetOwner()->GetComponent<TransformComponent>();
}

void dae::RotateComponent::Update(float deltaTime)
{
	if (!m_Transform)
		return;

	m_Angle += m_Speed * deltaTime;

	// Clamp angle to [0, 2π)
	if (m_Angle >= glm::two_pi<float>())
		m_Angle -= glm::two_pi<float>();
	else if (m_Angle < 0.0f)
		m_Angle += glm::two_pi<float>();

	const float x = m_Radius * glm::cos(m_Angle);
	const float y = m_Radius * glm::sin(m_Angle);

	m_Transform->SetLocalPosition(x, y);
}