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
		//Transform m_transform{};
		//std::shared_ptr<Texture2D> m_texture{};

		std::unordered_map<std::string, std::unique_ptr<Component>> m_Components;
	public:
		void Update(float deltaTime);
		void FixedUpdate(float fixedTimeStep);
		void Render() const;

		//void SetTexture(const std::string& filename);
		//void SetPosition(float x, float y);

		// Components
		Component* AddComponent(std::unique_ptr<Component> component);
		void RemoveComponent(const std::string& componentTypeName);
		Component* GetComponent(const std::string& componentTypeName);
		bool HasComponent(const std::string& componentTypeName);


		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
	};
}
