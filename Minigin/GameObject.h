#pragma once
#include <string>
#include <memory>
#include "Component.h"
#include <vector>
#include <typeindex>
#include <glm/glm.hpp>

namespace dae
{
	class Texture2D;
	class GameObject final
	{
	private:
		struct ComponentEntry
		{
			std::type_index typeIndex;
			std::unique_ptr<Component> component;
		};
		std::vector<ComponentEntry> m_Components;
		bool m_MarkedForDeath{ false };

		GameObject* m_Parent;
		std::vector<GameObject*> m_Children;
		glm::vec3 m_LocalPosition	{ };
		glm::vec3 m_WorldPosition	{ };
		bool m_PositionDirty		{ false };

		bool IsChild(GameObject* parent) const;


		void SetPositionDirty();
		void UpdateWorldPosition();

		void AddChild(GameObject* child);
		void RemoveChild(GameObject* child);

	public:
		void Update(float deltaTime);
		void FixedUpdate(float fixedTimeStep);
		void Render() const;

		// Scenegraph
		void SetParent(GameObject* parent, bool keepWorldPosition = false);
		void SetLocalPosition(const glm::vec3& pos);
		const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }
		const glm::vec3& GetWorldPosition();
		

		// Components
		template<typename ComponentType>
		ComponentType* AddComponent(std::unique_ptr<ComponentType> component); // moet anders??... component mag nog niet bestaan voor dat je het hier door geeft.
		
		template<typename ComponentType>
		void RemoveComponent();
		
		template<typename ComponentType>
		[[nodiscard]] ComponentType* GetComponent() const;
		
		template<typename ComponentType>
		bool HasComponent() const;

		void MarkForDeath();
		bool IsMarkedForDeath() const;


		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
	};

	template<typename ComponentType>
	ComponentType* GameObject::AddComponent(std::unique_ptr<ComponentType> component)
	{
		static_assert(std::is_base_of_v<Component, ComponentType>, "ComponentType must derive from Component");

		if (!component) return nullptr;

		std::type_index typeIndex{ typeid(ComponentType) };

		// Check if component already exists
		for (const auto& entry : m_Components)
		{
			if (entry.typeIndex == typeIndex)
				return nullptr;
		}

		ComponentType* componentPtr = component.get();
		m_Components.push_back({ typeIndex, std::move(component) });
		return componentPtr;
	}

	template<typename ComponentType>
	void GameObject::RemoveComponent()
	{
		static_assert(std::is_base_of_v<Component, ComponentType>, "ComponentType must derive from Component");

		std::type_index typeIndex{ typeid(ComponentType) };

		auto it = std::find_if(m_Components.begin(), m_Components.end(),
			[&typeIndex](const ComponentEntry& entry) { return entry.typeIndex == typeIndex; });

		if (it != m_Components.end())
		{
			m_Components.erase(it);
		}
	}

	template<typename ComponentType>
	ComponentType* GameObject::GetComponent() const
	{
		static_assert(std::is_base_of_v<Component, ComponentType>, "ComponentType must derive from Component");

		std::type_index typeIndex{ typeid(ComponentType) };

		for (const auto& entry : m_Components)
		{
			if (entry.typeIndex == typeIndex)
				return static_cast<ComponentType*>(entry.component.get());
		}

		return nullptr;
	}

	template<typename ComponentType>
	bool GameObject::HasComponent() const
	{
		static_assert(std::is_base_of_v<Component, ComponentType>, "ComponentType must derive from Component");

		std::type_index typeIndex{ typeid(ComponentType) };

		for (const auto& entry : m_Components)
		{
			if (entry.typeIndex == typeIndex)
				return true;
		}

		return false;
	}
}
