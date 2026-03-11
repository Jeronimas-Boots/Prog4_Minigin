#pragma once
#include "Command.h"

namespace dae
{
	class GameObject;

	class GameObjectCommand : public Command
	{
		GameObject* m_GameObject;

	protected:
		GameObject* GetGameObject() const { return m_GameObject; }

	public:
		GameObjectCommand(GameObject* object) : m_GameObject(object) { }
		virtual ~GameObjectCommand() = default;
	};
}