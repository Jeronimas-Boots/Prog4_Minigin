#include <string>
#include "GameObject.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float deltaTime)
{
	for (auto& component : m_Components)
	{
		component.second->Update(deltaTime);
	}
}

void dae::GameObject::FixedUpdate(float fixedTimeStep)
{
	for (auto& component : m_Components)
	{
		component.second->FixedUpdate(fixedTimeStep);
	}
}


void dae::GameObject::Render() const
{
	for (auto& component : m_Components)
	{
		component.second->Render();
	}
}

//void dae::GameObject::SetTexture(const std::string& filename)
//{
//	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
//}
//
//void dae::GameObject::SetPosition(float x, float y)
//{
//	m_transform.SetPosition(x, y, 0.0f);
//}

dae::Component* dae::GameObject::AddComponent(std::unique_ptr<Component> component)
{
	if (!component) return nullptr;
	std::string typeName = component->GetTypeName();
	if (m_Components.find(typeName) != m_Components.end()) return nullptr;

	Component* ptr = component.get();
	m_Components[typeName] = std::move(component);
	return ptr;
}

void dae::GameObject::RemoveComponent(const std::string& componentTypeName)
{
	m_Components.erase(componentTypeName);
}

dae::Component* dae::GameObject::GetComponent(const std::string& componentTypeName)
{
	auto comp = m_Components.find(componentTypeName);
	if (comp != m_Components.end())
		return comp->second.get();
	return nullptr;
}

bool dae::GameObject::HasComponent(const std::string& componentTypeName)
{

	return m_Components.find(componentTypeName) != m_Components.end();
}

void dae::GameObject::MarkForDeath()
{
	m_MarkedForDeath = true;
}

bool dae::GameObject::IsMarkedForDeath() const
{
	return m_MarkedForDeath;
}
