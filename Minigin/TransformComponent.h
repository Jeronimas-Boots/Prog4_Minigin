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

		void SetLocalPosition(float x, float y, float z = 0.f);
		const glm::vec3& GetWorldPosition() const;
		const glm::vec3& GetLocalPosition() const;
	};
}
