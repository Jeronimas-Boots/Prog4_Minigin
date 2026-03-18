#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include <memory>
#include "InputManager.h"
#include "MoveCommand.h"
#include "DamageCommand.h"
#include "HealthComponent.h"
#include "HealthUIComponent.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	// Background GameObject
	auto backgroundGO = std::make_unique<dae::GameObject>();
	backgroundGO->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(backgroundGO.get(), 0.f, 0.f, 0.f));
	backgroundGO->AddComponent<dae::RenderComponent>(std::make_unique<dae::RenderComponent>(
		backgroundGO.get(),
		dae::ResourceManager::GetInstance().LoadTexture("background.png")));
	scene.Add(std::move(backgroundGO));

	// FPS GameObject
	auto fontSmall = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto fpsGO = std::make_unique<dae::GameObject>();
	fpsGO->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(fpsGO.get(), 10.f, 10.f, 0.f));
	fpsGO->AddComponent<dae::RenderComponent>(std::make_unique<dae::RenderComponent>(fpsGO.get(), nullptr));
	fpsGO->AddComponent<dae::TextComponent>(std::make_unique<dae::TextComponent>(fpsGO.get(), "FPS: 0", SDL_Color{ 255, 255, 255, 255 }, fontSmall));
	fpsGO->AddComponent<dae::FPSComponent>(std::make_unique<dae::FPSComponent>(fpsGO.get()));
	scene.Add(std::move(fpsGO));

	// Logo GameObject
	auto logoGO = std::make_unique<dae::GameObject>();
	logoGO->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(logoGO.get(), 358.f, 180.f, 0.f));
	logoGO->AddComponent<dae::RenderComponent>(std::make_unique<dae::RenderComponent>(
		logoGO.get(),
		dae::ResourceManager::GetInstance().LoadTexture("logo.png")));
	scene.Add(std::move(logoGO));

	// Text GameObject
	auto fontLarge = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto textGO = std::make_unique<dae::GameObject>();
	textGO->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(textGO.get(), 292.f, 20.f, 0.f));
	textGO->AddComponent<dae::RenderComponent>(std::make_unique<dae::RenderComponent>(textGO.get(), nullptr));
	textGO->AddComponent<dae::TextComponent>(std::make_unique<dae::TextComponent>(textGO.get(), "Programming 4 Assignment", SDL_Color{ 255, 255, 0, 255 }, fontLarge));
	scene.Add(std::move(textGO));

	
	// Blue Tank
	auto blueTankGO = std::make_unique<dae::GameObject>();
	blueTankGO->AddComponent<dae::HealthComponent>(std::make_unique<dae::HealthComponent>(blueTankGO.get()));
	blueTankGO->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(blueTankGO.get(), 200.f, 300.f, 0.f));
	blueTankGO->AddComponent<dae::RenderComponent>(std::make_unique<dae::RenderComponent>(
		blueTankGO.get(),
		dae::ResourceManager::GetInstance().LoadTexture("BlueTank.png")));

	auto& input = dae::InputManager::GetInstance();
	constexpr float player1Speed = 100.f;

	// Player 1 - WASD keyboard controls
	input.BindCommand(SDL_SCANCODE_W, dae::KeyState::Pressed,
		std::make_unique<dae::MoveCommand>(blueTankGO.get(), glm::vec3{ 0.f, -1.f, 0.f }, player1Speed));
	input.BindCommand(SDL_SCANCODE_S, dae::KeyState::Pressed,
		std::make_unique<dae::MoveCommand>(blueTankGO.get(), glm::vec3{ 0.f, 1.f, 0.f }, player1Speed));
	input.BindCommand(SDL_SCANCODE_A, dae::KeyState::Pressed,
		std::make_unique<dae::MoveCommand>(blueTankGO.get(), glm::vec3{ -1.f, 0.f, 0.f }, player1Speed));
	input.BindCommand(SDL_SCANCODE_D, dae::KeyState::Pressed,
		std::make_unique<dae::MoveCommand>(blueTankGO.get(), glm::vec3{ 1.f, 0.f, 0.f }, player1Speed));
	input.BindCommand(SDL_SCANCODE_C, dae::KeyState::Up,
		std::make_unique<dae::DamageCommand>(blueTankGO.get()));

	// BlueTankUI
	auto blueTankHPUI = std::make_unique<dae::GameObject>();
	blueTankHPUI->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(blueTankHPUI.get(), 0.f, 140.f));
	blueTankHPUI->AddComponent<dae::TextComponent>(std::make_unique<dae::TextComponent>(blueTankHPUI.get(), "Lives: 0", SDL_Color{ 0, 0, 255, 255 }, fontSmall));
	blueTankHPUI->AddComponent<dae::RenderComponent>(std::make_unique<dae::RenderComponent>(blueTankHPUI.get(), nullptr));
	blueTankHPUI->AddComponent<dae::HealthUIComponent>(std::make_unique<dae::HealthUIComponent>(blueTankHPUI.get(), blueTankGO->GetComponent<dae::HealthComponent>()));
	

	scene.Add(std::move(blueTankHPUI));
	scene.Add(std::move(blueTankGO));


	// Red Tank
	auto redTankGO = std::make_unique<dae::GameObject>();
	redTankGO->AddComponent<dae::HealthComponent>(std::make_unique<dae::HealthComponent>(redTankGO.get()));
	redTankGO->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(redTankGO.get(), 250.f, 250.f, 0.f));
	redTankGO->AddComponent<dae::RenderComponent>(std::make_unique<dae::RenderComponent>(
		redTankGO.get(),
		dae::ResourceManager::GetInstance().LoadTexture("RedTank.png")));

	// Player 2 - Controller DPad controls (Controller 0)
	input.BindCommand(0, dae::ControllerButton::DPadUp, dae::KeyState::Pressed,
		std::make_unique<dae::MoveCommand>(redTankGO.get(), glm::vec3{ 0.f, -1.f, 0.f }, player1Speed * 2));
	input.BindCommand(0, dae::ControllerButton::DPadDown, dae::KeyState::Pressed,
		std::make_unique<dae::MoveCommand>(redTankGO.get(), glm::vec3{ 0.f, 1.f, 0.f }, player1Speed * 2));
	input.BindCommand(0, dae::ControllerButton::DPadLeft, dae::KeyState::Pressed,
		std::make_unique<dae::MoveCommand>(redTankGO.get(), glm::vec3{ -1.f, 0.f, 0.f }, player1Speed * 2));
	input.BindCommand(0, dae::ControllerButton::DPadRight, dae::KeyState::Pressed,
		std::make_unique<dae::MoveCommand>(redTankGO.get(), glm::vec3{ 1.f, 0.f, 0.f }, player1Speed * 2));

	// BlueTankUI
	auto redTankHPUI = std::make_unique<dae::GameObject>();
	redTankHPUI->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(redTankHPUI.get(), 0.f, 180.f));
	redTankHPUI->AddComponent<dae::TextComponent>(std::make_unique<dae::TextComponent>(redTankHPUI.get(), "Lives: 0", SDL_Color{ 255, 0, 0, 255 }, fontSmall));
	redTankHPUI->AddComponent<dae::RenderComponent>(std::make_unique<dae::RenderComponent>(redTankHPUI.get(), nullptr));
	redTankHPUI->AddComponent<dae::HealthUIComponent>(std::make_unique<dae::HealthUIComponent>(redTankHPUI.get(), redTankGO->GetComponent<dae::HealthComponent>()));

	scene.Add(std::move(redTankHPUI));
	scene.Add(std::move(redTankGO));


}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
