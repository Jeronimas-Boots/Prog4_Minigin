#include "Level0.h"
#include "LevelBuilder.h"
#include "GridCollisionComponent.h"
#include "GridMovementComponent.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "TankBuilder.h"
#include "inputManager.h"

void tron::Level0::Load(dae::Scene& scene, GameMode mode )
{
    dae::InputManager::GetInstance().ClearCommands();

    auto grid = dae::ResourceManager::GetInstance().LoadCSV(GetCSVPath());
    auto layout = tron::LevelBuilder::BuildScene(scene, grid);

    auto collisionGO = std::make_unique<dae::GameObject>();
    auto* collision = collisionGO->AddComponent<tron::GridCollisionComponent>(
        std::make_unique<tron::GridCollisionComponent>(
            collisionGO.get(), layout.grid,
            layout.tileSize, layout.offsetX, layout.offsetY));
    scene.Add(std::move(collisionGO));

    std::vector<glm::vec2> playerSpawns;
    for (int row = 0; row < static_cast<int>(layout.grid.size()) - 1; ++row)
    {
        for (int col = 0; col < static_cast<int>(layout.grid[row].size()) - 1; ++col)
        {
            if (layout.grid[row][col] == 6 &&
                layout.grid[row][col + 1] == 6 &&
                layout.grid[row + 1][col] == 6 &&
                layout.grid[row + 1][col + 1] == 6)
            {
                playerSpawns.push_back({
                    layout.offsetX + col * layout.tileSize,
                    layout.offsetY + row * layout.tileSize });
            }
        }
    }

    // Spawn players based on game mode
    if (!playerSpawns.empty())
        tron::CreatePlayer(scene, collision, layout,
            playerSpawns[0].x, playerSpawns[0].y, 0);

    if (mode == GameMode::CoOp || mode == GameMode::Versus)
        if (playerSpawns.size() >= 2)
            tron::CreatePlayer(scene, collision, layout,
                playerSpawns[1].x, playerSpawns[1].y, 1);
}