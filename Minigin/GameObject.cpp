#include "GameObject.h"
#include <algorithm>

dae::GameObject::~GameObject() = default;

bool dae::GameObject::IsChild(GameObject* parent) const
{
	for (auto& child : m_Children)
	{
		if (child == parent) return true;
	}
	return false;
}

void dae::GameObject::SetLocalPosition(const glm::vec3& pos)
{
	m_LocalPosition = pos;
	SetPositionDirty();
}

void dae::GameObject::SetPositionDirty()
{
	m_PositionDirty = true;
	for (auto& child : m_Children)
	{
		child->SetPositionDirty();
	}
}

void dae::GameObject::UpdateWorldPosition()
{
	if (m_PositionDirty)
	{
		if (m_Parent == nullptr)
			m_WorldPosition = m_LocalPosition;
		else
			m_WorldPosition = m_Parent->GetWorldPosition() + m_LocalPosition;
	}
	m_PositionDirty = false;
}

void dae::GameObject::AddChild(GameObject* child)
{
	m_Children.push_back(child);
}

void dae::GameObject::RemoveChild(GameObject* child)
{
	auto it = std::find(m_Children.begin(), m_Children.end(), child);
	if (it != m_Children.end())
	{
		m_Children.erase(it);
	}
}

void dae::GameObject::Update(float deltaTime)
{
	UpdateWorldPosition();
	for (auto& component : m_Components)
	{
		component.component->Update(deltaTime);
	}
}

void dae::GameObject::FixedUpdate(float fixedTimeStep)
{
	for (auto& component : m_Components)
	{
		component.component->FixedUpdate(fixedTimeStep);
	}
}


void dae::GameObject::Render() const
{
	for (auto& component : m_Components)
	{
		component.component->Render();
	}
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	if (IsChild(parent) || parent == this || m_Parent == parent)
		return;
	if (parent == nullptr)
		SetLocalPosition(GetWorldPosition());
	else
	{
		if (keepWorldPosition)
			SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
		SetPositionDirty();
	}
	if (m_Parent) m_Parent->RemoveChild(this);
	m_Parent = parent;
	if (m_Parent) m_Parent->AddChild(this);
}

const glm::vec3& dae::GameObject::GetWorldPosition()
{
	if (m_PositionDirty)
		UpdateWorldPosition();
	return m_WorldPosition;
}

void dae::GameObject::MarkForDeath()
{
	m_MarkedForDeath = true;
}

bool dae::GameObject::IsMarkedForDeath() const
{
	return m_MarkedForDeath;
}
