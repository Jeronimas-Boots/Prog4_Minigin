#pragma once
#include "Component.h"
#include "glm/glm.hpp"
#include <string>
namespace dae
{
	class TransformComponent final : public Component
	{
	public:
		TransformComponent(GameObject* owner, float x = 0.f, float y = 0.f, float z = 0.f);

		void SetPosition(float x, float y, float z = 0.f);
		const glm::vec3& GetPositioin() const;

		std::string GetTypeName() const override { return "TransformComponent"; }

	private:
		glm::vec3 m_Position;
	};
}
