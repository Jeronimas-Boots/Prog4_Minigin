#include "TransformComponent.h"

dae::TransformComponent::TransformComponent(GameObject* owner, float x, float y, float z)
	: Component(owner)
	, m_Position(x, y, z)
{

}

void dae::TransformComponent::SetPosition(float x, float y, float z)
{
	m_Position = { x, y, z };
}

const glm::vec3& dae::TransformComponent::GetPositioin() const
{
	return m_Position;
}