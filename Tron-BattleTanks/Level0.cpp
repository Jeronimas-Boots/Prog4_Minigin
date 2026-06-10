#include "Level0.h"
#include "LevelBuilder.h"
#include "GridCollisionComponent.h"
#include "GridMovementComponent.h"
#include "GridMoveCommand.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Scene.h"

void tron::Level0::Load(dae::Scene& scene)
{
    auto grid = dae::ResourceManager::GetInstance().LoadCSV(GetCSVPath());
    auto layout = tron::LevelBuilder::BuildScene(scene, grid);


    // Collision world — lives on a dedicated GameObject
    auto collisionGO = std::make_unique<dae::GameObject>();
    auto* collision = collisionGO->AddComponent<tron::GridCollisionComponent>(
        std::make_unique<tron::GridCollisionComponent>(
            collisionGO.get(), layout.grid,
            layout.tileSize, layout.offsetX, layout.offsetY));
    scene.Add(std::move(collisionGO));

    // Player
    auto playerGO = std::make_unique<dae::GameObject>();

    // Start on the first PlayerSpawn tile (tile value 6)
    float startX = layout.offsetX + layout.tileSize * 0.5f;
    float startY = layout.offsetY + layout.tileSize * 0.5f;
    for (int row = 0; row < static_cast<int>(layout.grid.size()); ++row)
        for (int col = 0; col < static_cast<int>(layout.grid[row].size()); ++col)
            if (layout.grid[row][col] == 6)
            {
                startX = layout.offsetX + col * layout.tileSize + layout.tileSize * 0.5f;
                startY = layout.offsetY + row * layout.tileSize + layout.tileSize * 0.5f;
                goto foundSpawn; // break out of both loops
            }
foundSpawn:

    playerGO->AddComponent<dae::TransformComponent>(
        std::make_unique<dae::TransformComponent>(playerGO.get(), startX, startY));
    playerGO->AddComponent<dae::RenderComponent>(
        std::make_unique<dae::RenderComponent>(
            playerGO.get(),
            dae::ResourceManager::GetInstance().LoadTexture("BlueTank.png")));

    auto* movement = playerGO->AddComponent<tron::GridMovementComponent>(
        std::make_unique<tron::GridMovementComponent>(playerGO.get(), collision,
            layout.tileSize * 8.f));

    // Scale the tank texture to match tile size
    playerGO->GetComponent<dae::RenderComponent>()->SetScale(layout.scale, layout.scale);

    scene.Add(std::move(playerGO));

    // Input — keyboard
    auto& input = dae::InputManager::GetInstance();
    input.BindCommand(SDL_SCANCODE_W, dae::KeyState::Pressed,
        std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ 0.f, -1.f }));
    input.BindCommand(SDL_SCANCODE_S, dae::KeyState::Pressed,
        std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ 0.f,  1.f }));
    input.BindCommand(SDL_SCANCODE_A, dae::KeyState::Pressed,
        std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ -1.f, 0.f }));
    input.BindCommand(SDL_SCANCODE_D, dae::KeyState::Pressed,
        std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ 1.f, 0.f }));

    // Input — controller
    input.BindCommand(0, dae::ControllerButton::DPadUp, dae::KeyState::Pressed,
        std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ 0.f, -1.f }));
    input.BindCommand(0, dae::ControllerButton::DPadDown, dae::KeyState::Pressed,
        std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ 0.f,  1.f }));
    input.BindCommand(0, dae::ControllerButton::DPadLeft, dae::KeyState::Pressed,
        std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ -1.f, 0.f }));
    input.BindCommand(0, dae::ControllerButton::DPadRight, dae::KeyState::Pressed,
        std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ 1.f, 0.f }));
}