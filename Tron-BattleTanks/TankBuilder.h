#pragma once
#include <memory>

namespace dae { class GameObject; class Scene; }

namespace tron
{
	class GridCollisionComponent; 
	struct LevelLayout;

	dae::GameObject* CreatePlayer(dae::Scene& scene, GridCollisionComponent* collision, const LevelLayout& layout, float spawnX, float spawnY, int playerIndex);
}