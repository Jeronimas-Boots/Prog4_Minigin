#include <algorithm>
#include "Scene.h"
#include <assert.h>

using namespace dae;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(const GameObject& object)
{
	for (auto& obj : m_objects)
	{
		if (obj.get() == &object)
		{
			obj->MarkForDeath();
		}
	}
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update(float deltaTime)
{
	for(auto& object : m_objects)
	{
		if (!object->IsMarkedForDeath())
		{
			object->Update(deltaTime);
		}
	}
	DestroyMarkedObjects();
}

void dae::Scene::FixedUpdate(float fixedTimeStep)
{
	for (auto& object : m_objects)
	{
		if (!object->IsMarkedForDeath())
		{
			object->FixedUpdate(fixedTimeStep);
		}
	}
	DestroyMarkedObjects();
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		if (!object->IsMarkedForDeath())
		{
			object->Render();
		}
	}
}

void dae::Scene::DestroyMarkedObjects()
{
	m_objects.erase(
		std::remove_if(
			m_objects.begin(),
			m_objects.end(),
			[](const auto& ptr) { return ptr->IsMarkedForDeath(); }
		),
		m_objects.end()
	);
}

