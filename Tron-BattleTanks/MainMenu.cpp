#include "MainMenu.h"
#include "MenuStateComponent.h"
#include "MenuCommands.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "TextComponent.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "SDL_MixerSoundSystem.h"

tron::MainMenu::MainMenu(std::function<void(GameMode)> onStart, std::function<void()> onQuit)
    : m_OnStart(std::move(onStart))
    , m_OnQuit(std::move(onQuit))
{
}

void tron::MainMenu::Load(dae::Scene& scene, GameMode /*mode*/, LevelManager* /*levelManager*/)
{
    dae::InputManager::GetInstance().ClearCommands();

    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 28);

    constexpr float centerX = 512.f;
    constexpr float startY = 150.f;
    constexpr float spacing = 60.f;

    // Background
    auto backgroundGO = std::make_unique<dae::GameObject>();
    backgroundGO->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(backgroundGO.get(), 0.f, 0.f, 0.f));
    backgroundGO->AddComponent<dae::RenderComponent>(std::make_unique<dae::RenderComponent>(
        backgroundGO.get(),
        dae::ResourceManager::GetInstance().LoadTexture("background.png")));
    scene.Add(std::move(backgroundGO));


    // Title
    auto titleGO = std::make_unique<dae::GameObject>();
    titleGO->AddComponent<dae::TransformComponent>(
        std::make_unique<dae::TransformComponent>(titleGO.get(), centerX - 100.f, startY));
    titleGO->AddComponent<dae::RenderComponent>(
        std::make_unique<dae::RenderComponent>(titleGO.get(), nullptr));
    titleGO->AddComponent<dae::TextComponent>(
        std::make_unique<dae::TextComponent>(titleGO.get(), "TRON Battle Tanks",
            SDL_Color{ 0, 255, 255, 255 }, font));
    scene.Add(std::move(titleGO));

    // Start instruction
    auto startGO = std::make_unique<dae::GameObject>();
    startGO->AddComponent<dae::TransformComponent>(
        std::make_unique<dae::TransformComponent>(startGO.get(), centerX - 150.f, startY + spacing));
    startGO->AddComponent<dae::RenderComponent>(
        std::make_unique<dae::RenderComponent>(startGO.get(), nullptr));
    startGO->AddComponent<dae::TextComponent>(
        std::make_unique<dae::TextComponent>(startGO.get(), "A / Space  -  Start",
            SDL_Color{ 255, 255, 255, 255 }, font));
    scene.Add(std::move(startGO));

    // Quit instruction
    auto quitGO = std::make_unique<dae::GameObject>();
    quitGO->AddComponent<dae::TransformComponent>(
        std::make_unique<dae::TransformComponent>(quitGO.get(), centerX - 150.f, startY + spacing * 2));
    quitGO->AddComponent<dae::RenderComponent>(
        std::make_unique<dae::RenderComponent>(quitGO.get(), nullptr));
    quitGO->AddComponent<dae::TextComponent>(
        std::make_unique<dae::TextComponent>(quitGO.get(), "B / Esc  -  Quit",
            SDL_Color{ 255, 255, 255, 255 }, font));
    scene.Add(std::move(quitGO));

    // Toggle instruction
    auto toggleGO = std::make_unique<dae::GameObject>();
    toggleGO->AddComponent<dae::TransformComponent>(
        std::make_unique<dae::TransformComponent>(toggleGO.get(), centerX - 150.f, startY + spacing * 3));
    toggleGO->AddComponent<dae::RenderComponent>(
        std::make_unique<dae::RenderComponent>(toggleGO.get(), nullptr));
    toggleGO->AddComponent<dae::TextComponent>(
        std::make_unique<dae::TextComponent>(toggleGO.get(), "Y / Tab  -  Change Mode",
            SDL_Color{ 255, 255, 255, 255 }, font));
    scene.Add(std::move(toggleGO));

    // Game mode display — owned by the menu state component
    auto modeGO = std::make_unique<dae::GameObject>();
    modeGO->AddComponent<dae::TransformComponent>(
        std::make_unique<dae::TransformComponent>(modeGO.get(), centerX - 80.f, startY + spacing * 4));
    modeGO->AddComponent<dae::RenderComponent>(
        std::make_unique<dae::RenderComponent>(modeGO.get(), nullptr));
    auto* modeText = modeGO->AddComponent<dae::TextComponent>(
        std::make_unique<dae::TextComponent>(modeGO.get(), "Mode: Singleplayer",
            SDL_Color{ 255, 255, 0, 255 }, font));

    // Menu state component
    auto* menuState = modeGO->AddComponent<tron::MenuStateComponent>(
        std::make_unique<tron::MenuStateComponent>(
            modeGO.get(), modeText, m_OnStart, m_OnQuit));

    scene.Add(std::move(modeGO));

    // Input
    auto& input = dae::InputManager::GetInstance();

    input.BindCommand(SDL_SCANCODE_SPACE, dae::KeyState::Down,
        std::make_unique<tron::StartCommand>(menuState));
    input.BindCommand(SDL_SCANCODE_ESCAPE, dae::KeyState::Down,
        std::make_unique<tron::QuitCommand>(menuState));
    input.BindCommand(SDL_SCANCODE_TAB, dae::KeyState::Down,
        std::make_unique<tron::ToggleGameModeCommand>(menuState));

    input.BindCommand(0, dae::ControllerButton::ButtonA, dae::KeyState::Down,
        std::make_unique<tron::StartCommand>(menuState));
    input.BindCommand(0, dae::ControllerButton::ButtonB, dae::KeyState::Down,
        std::make_unique<tron::QuitCommand>(menuState));
    input.BindCommand(0, dae::ControllerButton::ButtonY, dae::KeyState::Down,
        std::make_unique<tron::ToggleGameModeCommand>(menuState));
}