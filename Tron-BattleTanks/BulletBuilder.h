#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace dae { class Scene; class GameObject; }
namespace tron { class GridCollisionComponent; }

namespace tron
{
    void SpawnBullet(dae::Scene& scene, GridCollisionComponent* collision, const glm::vec3& position, const glm::vec2& direction, float renderAngle, float tileSize, float scale, const dae::GameObject* shooter, std::vector<dae::GameObject*> targets);

    void SpawnExplosion(dae::Scene& scene, const glm::vec3& position, float tileSize, float scale);
}