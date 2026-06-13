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
#include "GridCollisionComponent.h"
#include "HealthComponent.h"
#include "RectColliderComponent.h"
#include "RespawnComponent.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "HealthUIComponent.h"
#include "ScoreUIComponent.h"
#include "LevelManager.h"
#include "NextLevelCommand.h"

static tron::GunComponent* CreateGun(dae::Scene& scene, dae::GameObject* tankGO, tron::GridCollisionComponent* collision, const tron::LevelLayout& layout, int playerIndex)

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
        std::make_unique<tron::GunComponent>(
            gunGO.get(), playerIndex, &scene, collision, layout.tileSize, layout.scale));

    gunGO->SetParent(tankGO, false);

    auto* gunComp = gunGO->GetComponent<tron::GunComponent>();
    scene.Add(std::move(gunGO));
    return gunComp;
}

tron::PlayerTank tron::CreatePlayer(dae::Scene& scene, tron::GridCollisionComponent* collision,
    const tron::LevelLayout& layout, float spawnX, float spawnY, int playerIndex) 
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

    auto* health = playerGO->AddComponent<dae::HealthComponent>(
        std::make_unique<dae::HealthComponent>(playerGO.get(), 3));

    playerGO->AddComponent<dae::RectColliderComponent>(
        std::make_unique<dae::RectColliderComponent>(playerGO.get(), 0.f, 0.f, "player"));
    playerGO->AddComponent<dae::ScoreComponent>(std::make_unique<dae::ScoreComponent>(playerGO.get()));


    auto fontSmall = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

    // Bind input based on player index
    auto& input = dae::InputManager::GetInstance();
    if (playerIndex == 0)
    {
        auto player1HPUI = std::make_unique<dae::GameObject>();
        player1HPUI->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(player1HPUI.get(), 0.f, 20.f));
        player1HPUI->AddComponent<dae::TextComponent>(std::make_unique<dae::TextComponent>(player1HPUI.get(), "Lives: 0", SDL_Color{ 0, 0, 255, 255 }, fontSmall));
        player1HPUI->AddComponent<dae::RenderComponent>(std::make_unique<dae::RenderComponent>(player1HPUI.get(), nullptr));
        player1HPUI->AddComponent<dae::HealthUIComponent>(std::make_unique<dae::HealthUIComponent>(player1HPUI.get(), playerGO->GetComponent<dae::HealthComponent>()));

        auto player1ScoreUI = std::make_unique<dae::GameObject>();
        player1ScoreUI->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(player1ScoreUI.get(), 0.f, 40.f));
        player1ScoreUI->AddComponent<dae::TextComponent>(std::make_unique<dae::TextComponent>(player1ScoreUI.get(), "Score: 0", SDL_Color{ 255, 0, 0, 255 }, fontSmall));
        player1ScoreUI->AddComponent<dae::RenderComponent>(std::make_unique<dae::RenderComponent>(player1ScoreUI.get(), nullptr));
        player1ScoreUI->AddComponent<dae::ScoreUIComponent>(std::make_unique<dae::ScoreUIComponent>(player1ScoreUI.get(), playerGO->GetComponent<dae::ScoreComponent>()));

        scene.Add(std::move(player1HPUI));
        scene.Add(std::move(player1ScoreUI));
        
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
        auto player2HPUI = std::make_unique<dae::GameObject>();
        player2HPUI->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(player2HPUI.get(), 850.f, 20.f));
        player2HPUI->AddComponent<dae::TextComponent>(std::make_unique<dae::TextComponent>(player2HPUI.get(), "Lives: 0", SDL_Color{ 255, 0, 0, 255 }, fontSmall));
        player2HPUI->AddComponent<dae::RenderComponent>(std::make_unique<dae::RenderComponent>(player2HPUI.get(), nullptr));
        player2HPUI->AddComponent<dae::HealthUIComponent>(std::make_unique<dae::HealthUIComponent>(player2HPUI.get(), playerGO->GetComponent<dae::HealthComponent>()));
        
        auto player2ScoreUI = std::make_unique<dae::GameObject>();
        player2ScoreUI->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(player2ScoreUI.get(), 850.f, 40.f));
        player2ScoreUI->AddComponent<dae::TextComponent>(std::make_unique<dae::TextComponent>(player2ScoreUI.get(), "Score: 0", SDL_Color{ 255, 0, 0, 255 }, fontSmall));
        player2ScoreUI->AddComponent<dae::RenderComponent>(std::make_unique<dae::RenderComponent>(player2ScoreUI.get(), nullptr));
        player2ScoreUI->AddComponent<dae::ScoreUIComponent>(std::make_unique<dae::ScoreUIComponent>(player2ScoreUI.get(), playerGO->GetComponent<dae::ScoreComponent>()));


        scene.Add(std::move(player2HPUI));
        scene.Add(std::move(player2ScoreUI));
        
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
    scene.Add(std::move(playerGO));

    // ADD: respawn (after scene.Add so the component can find the transform)
    ptr->AddComponent<tron::RespawnComponent>(
        std::make_unique<tron::RespawnComponent>(ptr, health, glm::vec2{ spawnX, spawnY }));

    auto* gun = CreateGun(scene, ptr, collision, layout, playerIndex);
    return { ptr, gun };
}

