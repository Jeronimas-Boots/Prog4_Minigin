#include "EnemyBuilder.h"
#include "GameObject.h"
#include "Scene.h"
#include "LevelBuilder.h"
#include "GridMovementComponent.h"
#include "GridCollisionComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "GunComponent.h"
#include "TankStateComponent.h"
#include "StatesTank.h"


// Finds the top-left grid coordinate (col, row) of every 2x2 block of EnemyTankSpawn tiles.
std::vector<std::pair<int, int>> FindEnemyTankSpawnBlocks(const std::vector<std::vector<int>>& grid)
{
    std::vector<std::pair<int, int>> blocks;

    const int rows = static_cast<int>(grid.size());

    for (int row = 0; row < rows - 1; ++row)
    {
        const int cols = static_cast<int>(grid[row].size());
        for (int col = 0; col < cols - 1; ++col)
        {
            const auto tile = static_cast<tron::TileType>(grid[row][col]);
            const auto tileRight = static_cast<tron::TileType>(grid[row][col + 1]);
            const auto tileDown = static_cast<tron::TileType>(grid[row + 1][col]);
            const auto tileDiag = static_cast<tron::TileType>(grid[row + 1][col + 1]);

            const bool isBlock =
                tile == tron::TileType::EnemyTankSpawn &&
                tileRight == tron::TileType::EnemyTankSpawn &&
                tileDown == tron::TileType::EnemyTankSpawn &&
                tileDiag == tron::TileType::EnemyTankSpawn;

            if (isBlock)
            {
                blocks.emplace_back(col, row);

                // Skip past this block's second column so we don't also match
                // its right half as the start of an overlapping block.
                ++col;
            }
        }
    }

    return blocks;
}


static dae::GameObject* CreateEnemyGun(dae::Scene& scene, dae::GameObject* tankGO, tron::GridCollisionComponent* collision, const tron::LevelLayout& layout)
{
    auto gunGO = std::make_unique<dae::GameObject>();

    // Center the 48x48 gun on the 32x32 tank (same as player guns)
    const float centerOffset = -8.f * layout.scale;

    gunGO->AddComponent<dae::TransformComponent>(
        std::make_unique<dae::TransformComponent>(gunGO.get(), centerOffset, centerOffset));

    gunGO->AddComponent<dae::RenderComponent>(
        std::make_unique<dae::RenderComponent>(
            gunGO.get(),
            dae::ResourceManager::GetInstance().LoadTexture("GreenTankGun.png")));

    gunGO->GetComponent<dae::RenderComponent>()->SetScale(layout.scale, layout.scale);

    // controllerIndex = -1 marks this as an enemy gun (no stick input, fired via Shoot())
    gunGO->AddComponent<tron::GunComponent>(
        std::make_unique<tron::GunComponent>(
            gunGO.get(), -1, &scene, collision, layout.tileSize, layout.scale));

    gunGO->SetParent(tankGO, false);

    dae::GameObject* ptr = gunGO.get();
    scene.Add(std::move(gunGO));
    return ptr;
}

std::vector<dae::GameObject*> tron::CreateEnemyTanks(dae::Scene& scene, GridCollisionComponent* collision, const LevelLayout& layout)
{
    std::vector<dae::GameObject*> spawnedTanks;

    // layout.tileSize is already TILE_SIZE * scale (the scaled tile size in world units)
    for (const auto& [col, row] : FindEnemyTankSpawnBlocks(layout.grid))
    {
        // Top-left corner of the 2x2 spawn block in world space
        const float spawnX = layout.offsetX + col * layout.tileSize;
        const float spawnY = layout.offsetY + row * layout.tileSize;

        auto enemyGO = std::make_unique<dae::GameObject>();

        enemyGO->AddComponent<dae::TransformComponent>(
            std::make_unique<dae::TransformComponent>(enemyGO.get(), spawnX, spawnY));

        enemyGO->AddComponent<dae::RenderComponent>(
            std::make_unique<dae::RenderComponent>(
                enemyGO.get(),
                dae::ResourceManager::GetInstance().LoadTexture("GreenTank.png")));

        enemyGO->GetComponent<dae::RenderComponent>()->SetScale(layout.scale, layout.scale);

        auto* movement = enemyGO->AddComponent<tron::GridMovementComponent>(
            std::make_unique<tron::GridMovementComponent>(
                enemyGO.get(), collision, layout.tileSize * 6.f)); // slightly slower than player

        dae::GameObject* ptr = enemyGO.get();
        scene.Add(std::move(enemyGO)); // must be in scene before adding gun as a child

        auto* gun = CreateEnemyGun(scene, ptr, collision, layout);
        auto* gunComponent = gun->GetComponent<tron::GunComponent>();

        ptr->AddComponent<tron::TankStateComponent>(
            std::make_unique<tron::TankStateComponent>(
                ptr, std::make_unique<tron::TankWanderState>(), gunComponent, collision, movement));

        spawnedTanks.push_back(ptr);
    }

    return spawnedTanks;
}