#pragma once
#include <string>
namespace dae
{
	class GameObject;
	class Component
	{
	public:
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Update(float /*deltaTime*/) {};
		virtual void FixedUpdate(float /*fixedTimeStep*/) {};
		virtual void Render() const {};

		virtual std::string GetTypeName() const = 0;

	protected:
		explicit Component(GameObject* owner) : m_Owner(owner) {}
		GameObject* GetOwner() const { return m_Owner; }

	private:
		GameObject* m_Owner;
	};
}