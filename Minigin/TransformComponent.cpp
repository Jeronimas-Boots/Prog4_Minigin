#include "TransformComponent.h"
#include "GameObject.h"

dae::TransformComponent::TransformComponent(GameObject* owner, float x, float y, float z)
	: Component(owner)
{
	owner->SetLocalPosition({ x, y, z });
}

void dae::TransformComponent::SetLocalPosition(float x, float y, float z)
{
	GetOwner()->SetLocalPosition({ x, y, z });
}

const glm::vec3& dae::TransformComponent::GetWorldPosition() const
{
	return GetOwner()->GetWorldPosition();
}

const glm::vec3& dae::TransformComponent::GetLocalPosition() const
{
	return GetOwner()->GetLocalPosition();
}