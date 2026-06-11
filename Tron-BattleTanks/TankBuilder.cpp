#include "TankBuilder.h"
#include "GameObject.h"
#include "Scene.h"
#include "LevelBuilder.h"
#include "GridMovementComponent.h"
#include "GridMoveCommand.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "InputManager.h"

dae::GameObject* tron::CreatePlayer(dae::Scene& scene, tron::GridCollisionComponent* collision, const tron::LevelLayout& layout, float spawnX, float spawnY, int playerIndex)
{
    auto playerGO = std::make_unique<dae::GameObject>();

    const std::string texture = playerIndex == 0 ? "BlueTank.png" : "RedTank.png";

    playerGO->AddComponent<dae::TransformComponent>(
        std::make_unique<dae::TransformComponent>(playerGO.get(), spawnX, spawnY));

    playerGO->AddComponent<dae::RenderComponent>(
        std::make_unique<dae::RenderComponent>(
            playerGO.get(),
            dae::ResourceManager::GetInstance().LoadTexture(texture)));

    playerGO->GetComponent<dae::RenderComponent>()->SetScale(layout.scale, layout.scale);

    auto* movement = playerGO->AddComponent<tron::GridMovementComponent>(
        std::make_unique<tron::GridMovementComponent>(
            playerGO.get(), collision, layout.tileSize * 8.f));

    // Bind input based on player index
    auto& input = dae::InputManager::GetInstance();
    if (playerIndex == 0)
    {
        input.BindCommand(SDL_SCANCODE_W, dae::KeyState::Pressed,
            std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ 0.f, -1.f }));
        input.BindCommand(SDL_SCANCODE_S, dae::KeyState::Pressed,
            std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ 0.f,  1.f }));
        input.BindCommand(SDL_SCANCODE_A, dae::KeyState::Pressed,
            std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ -1.f, 0.f }));
        input.BindCommand(SDL_SCANCODE_D, dae::KeyState::Pressed,
            std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ 1.f, 0.f }));
    }
    else
    {
        input.BindCommand(0, dae::ControllerButton::DPadUp, dae::KeyState::Pressed,
            std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ 0.f, -1.f }));
        input.BindCommand(0, dae::ControllerButton::DPadDown, dae::KeyState::Pressed,
            std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ 0.f,  1.f }));
        input.BindCommand(0, dae::ControllerButton::DPadLeft, dae::KeyState::Pressed,
            std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ -1.f, 0.f }));
        input.BindCommand(0, dae::ControllerButton::DPadRight, dae::KeyState::Pressed,
            std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ 1.f, 0.f }));
    }

    dae::GameObject* ptr = playerGO.get();
    scene.Add(std::move(playerGO));
    return ptr;
}
