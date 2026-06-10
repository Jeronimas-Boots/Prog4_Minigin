#pragma once	
#include <vector>
#include <string>

namespace dae { class Scene; }

namespace tron
{
    enum class TileType // numbers are the same as in the excel sheet used to layout the level
    {
        Wall = 0,
        Void = 1,
        Path = 2,
        Teleport = 3,
        EnemySpawn = 4,
        PlayerSpawn = 6
    };

    struct LevelLayout
    {
        std::vector<std::vector<int>> grid;
        float tileSize;
        float offsetX;
        float offsetY;
        float scale;
    };

	class LevelBuilder
	{
	public:
        static LevelLayout BuildScene(dae::Scene& scene, const std::vector<std::vector<int>>& grid);

    private:
        static constexpr float TILE_SIZE = 8.f;

        static void SpawnTile(dae::Scene& scene, float x, float y, float scale, const std::string& tilePng);
	};
}