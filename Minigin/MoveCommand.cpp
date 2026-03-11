#include "MoveCommand.h"
#include "GameObject.h"
#include "TransformComponent.h"

dae::MoveCommand::MoveCommand(GameObject* gameObject, const glm::vec3& direction, float speed)
	: GameObjectCommand(gameObject)
	, m_Direction(direction)
	, m_Speed(speed)
{ }

void dae::MoveCommand::Execute(float deltaTime)
{
	TransformComponent* transform = GetGameObject()->GetComponent<TransformComponent>();
	if (transform)
	{
		const glm::vec3 currentpos = transform->GetLocalPosition();
		transform->SetLocalPosition(
			currentpos.x + m_Direction.x * m_Speed * deltaTime,
			currentpos.y + m_Direction.y * m_Speed * deltaTime,
			currentpos.z + m_Direction.z * m_Speed * deltaTime
		);
	}
}

