#include "Level0.h"
#include "LevelBuilder.h"
#include "GridCollisionComponent.h"
#include "GridMovementComponent.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "TankBuilder.h"
#include "InputManager.h"
#include "EnemyBuilder.h"
#include "TankStateComponent.h"
#include "ServiceLocator.h"
#include "SDL_MixerSoundSystem.h"
#include "GunComponent.h"
#include "LevelManager.h"
#include "NextLevelCommand.h"

tron::Level0::Level0(const std::string& levelPath)
    : m_LevelPath(levelPath)
{
}

void tron::Level0::Load(dae::Scene& scene, GameMode mode, LevelManager* levelManager)
{
    

    auto backgroundGO = std::make_unique<dae::GameObject>();
    backgroundGO->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(backgroundGO.get(), 0.f, 0.f, 0.f));
    backgroundGO->AddComponent<dae::RenderComponent>(std::make_unique<dae::RenderComponent>(
        backgroundGO.get(),
        dae::ResourceManager::GetInstance().LoadTexture("background.png")));
    scene.Add(std::move(backgroundGO));

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
        for (int col = 0; col < static_cast<int>(layout.grid[row].size()) - 1; ++col)
            if (layout.grid[row][col] == 6 &&
                layout.grid[row][col + 1] == 6 &&
                layout.grid[row + 1][col] == 6 &&
                layout.grid[row + 1][col + 1] == 6)
                playerSpawns.push_back({
                    layout.offsetX + col * layout.tileSize,
                    layout.offsetY + row * layout.tileSize });

    std::vector<tron::PlayerTank> playerTanks;
    std::vector<dae::GameObject*> players;

    if (!playerSpawns.empty())
    {
        auto pt = tron::CreatePlayer(scene, collision, layout,
            playerSpawns[0].x, playerSpawns[0].y, 0);
        players.push_back(pt.tank);
        playerTanks.push_back(pt);
    }

    if (mode == GameMode::CoOp || mode == GameMode::Versus)
        if (playerSpawns.size() >= 2)
        {
            auto pt = tron::CreatePlayer(scene, collision, layout,
                playerSpawns[1].x, playerSpawns[1].y, 1);
            players.push_back(pt.tank);
            playerTanks.push_back(pt);
        }

    auto enemyTanks = tron::CreateEnemyTanks(scene, collision, layout);

    std::vector<dae::GameObject*> enemyGameObjects;
    for (auto& et : enemyTanks)
    {
        enemyGameObjects.push_back(et.tank);
        et.tank->GetComponent<tron::TankStateComponent>()->SetTargets(players);

        if (et.gun)
            et.gun->SetBulletTargets(players);
    }

    for (auto& pt : playerTanks)
        if (pt.gun)
            pt.gun->SetBulletTargets(enemyGameObjects);

    // Debug: cycle levels
    auto& input = dae::InputManager::GetInstance();
    input.BindCommand(SDL_SCANCODE_F1, dae::KeyState::Down,
        std::make_unique<tron::NextLevelCommand>(levelManager));
}