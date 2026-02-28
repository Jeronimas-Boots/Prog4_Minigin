#pragma once
#include "Component.h"
#include "glm/glm.hpp"

namespace dae
{
	class TransformComponent;

	class RotateComponent final : public Component
	{
	public:
		RotateComponent(GameObject* owner, float radius, float speed);

		void Update(float deltaTime) override;

		void SetRadius(float radius) { m_Radius = radius; }
		void SetSpeed(float speed) { m_Speed = speed; }
		float GetRadius() const { return m_Radius; }
		float GetSpeed() const { return m_Speed; }

	private:
		float m_Radius;
		float m_Speed;
		float m_Angle{ 0.f };
		glm::vec2 m_CenterPosition{ 0.f, 0.f };
		TransformComponent* m_Transform{ nullptr };
	};
}