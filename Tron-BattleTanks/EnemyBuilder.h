#pragma once
#include <memory>
#include <vector>

namespace dae { class GameObject; class Scene; }

namespace tron
{
	class GunComponent;
	struct EnemyTank
	{
		dae::GameObject* tank;
		GunComponent* gun;
	};

	class GridCollisionComponent;
	struct LevelLayout;

	std::vector<EnemyTank> CreateEnemyTanks(dae::Scene& scene, GridCollisionComponent* collision, const LevelLayout& layout);
}