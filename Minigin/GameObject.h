#pragma once
#include <string>
#include <memory>
#include "Component.h"
#include <unordered_map>

namespace dae
{
	class Texture2D;
	class GameObject final
	{
	private:
		std::unordered_map<std::string, std::unique_ptr<Component>> m_Components;
		bool m_MarkedForDeath{ false };

	public:
		void Update(float deltaTime);
		void FixedUpdate(float fixedTimeStep);
		void Render() const;

		// Components
		Component* AddComponent(std::unique_ptr<Component> component);
		void RemoveComponent(const std::string& componentTypeName);
		Component* GetComponent(const std::string& componentTypeName);
		bool HasComponent(const std::string& componentTypeName);

		void MarkForDeath();
		bool IsMarkedForDeath() const;


		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
	};
}
