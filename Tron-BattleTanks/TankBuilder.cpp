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
#include "GunComponent.h"

static dae::GameObject* CreateGun(dae::Scene& scene, dae::GameObject* tankGO, const tron::LevelLayout& layout, int playerIndex)
{
    const std::string texture = playerIndex == 0 ? "BlueTankGun.png" : "RedTankGun.png";

    auto gunGO = std::make_unique<dae::GameObject>();

    // Center the 48x48 gun on the 32x32 tank
    // Offset = -((gunSize - tankSize) / 2) * scale = -(8) * scale
    const float centerOffset = -8.f * layout.scale;

    gunGO->AddComponent<dae::TransformComponent>(
        std::make_unique<dae::TransformComponent>(gunGO.get(), centerOffset, centerOffset));

    gunGO->AddComponent<dae::RenderComponent>(
        std::make_unique<dae::RenderComponent>(
            gunGO.get(),
            dae::ResourceManager::GetInstance().LoadTexture(texture)));

    gunGO->GetComponent<dae::RenderComponent>()->SetScale(layout.scale, layout.scale);

    gunGO->AddComponent<tron::GunComponent>(
        std::make_unique<tron::GunComponent>(gunGO.get(), playerIndex));

    gunGO->SetParent(tankGO, false);

    dae::GameObject* ptr = gunGO.get();
    scene.Add(std::move(gunGO));
    return ptr;
}

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

        input.BindCommand(0, dae::ControllerButton::DPadUp, dae::KeyState::Pressed,
            std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ 0.f, -1.f }));
        input.BindCommand(0, dae::ControllerButton::DPadDown, dae::KeyState::Pressed,
            std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ 0.f,  1.f }));
        input.BindCommand(0, dae::ControllerButton::DPadLeft, dae::KeyState::Pressed,
            std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ -1.f, 0.f }));
        input.BindCommand(0, dae::ControllerButton::DPadRight, dae::KeyState::Pressed,
            std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ 1.f, 0.f }));
    }
    else
    {
        input.BindCommand(SDL_SCANCODE_UP, dae::KeyState::Pressed,
            std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ 0.f, -1.f }));
        input.BindCommand(SDL_SCANCODE_DOWN, dae::KeyState::Pressed,
            std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ 0.f,  1.f }));
        input.BindCommand(SDL_SCANCODE_LEFT, dae::KeyState::Pressed,
            std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ -1.f, 0.f }));
        input.BindCommand(SDL_SCANCODE_RIGHT, dae::KeyState::Pressed,
            std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ 1.f, 0.f }));

        input.BindCommand(1, dae::ControllerButton::DPadUp, dae::KeyState::Pressed,
            std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ 0.f, -1.f }));
        input.BindCommand(1, dae::ControllerButton::DPadDown, dae::KeyState::Pressed,
            std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ 0.f,  1.f }));
        input.BindCommand(1, dae::ControllerButton::DPadLeft, dae::KeyState::Pressed,
            std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ -1.f, 0.f }));
        input.BindCommand(1, dae::ControllerButton::DPadRight, dae::KeyState::Pressed,
            std::make_unique<tron::GridMoveCommand>(movement, glm::vec2{ 1.f, 0.f }));
    }

    dae::GameObject* ptr = playerGO.get();
    scene.Add(std::move(playerGO)); // tank must be in scene before setting parent
    CreateGun(scene, ptr, layout, playerIndex);
    return ptr;
}

