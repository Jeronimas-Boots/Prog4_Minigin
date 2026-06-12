#pragma once
#include <memory>
#include <vector>

namespace dae { class GameObject; class Scene; }

namespace tron
{
	class GridCollisionComponent;
	struct LevelLayout;

	std::vector<dae::GameObject*> CreateEnemyTanks(dae::Scene& scene, GridCollisionComponent* collision, const LevelLayout& layout);
}