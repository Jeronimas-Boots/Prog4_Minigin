#pragma once
#include "glm/glm.hpp"
#include "GameObjectCommand.h"

namespace dae
{
	class MoveCommand final : public GameObjectCommand
	{
	public:
		MoveCommand(GameObject* gameObject, const glm::vec3& direction, float speed);

		void Execute(float deltaTime) override;

	private:
		glm::vec3 m_Direction;
		float m_Speed;
	};
}