#pragma once
#include <glm/glm.hpp>

namespace dae { class Scene; }
namespace tron { class GridCollisionComponent; }

namespace tron
{
    void SpawnBullet(dae::Scene& scene, GridCollisionComponent* collision, const glm::vec3& position, const glm::vec2& direction, float renderAngle, float tileSize, float scale);

    void SpawnExplosion(dae::Scene& scene, const glm::vec3& position, float tileSize, float scale);
}
