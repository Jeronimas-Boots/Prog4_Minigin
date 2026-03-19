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
#include "ScoreUIComponent.h"
#include "ScoreComponent.h"
#include "ScoreCommand.h"
#include "Achievements.h"
#include "AchievementObserver.h"

#include <filesystem>
namespace fs = std::filesystem;

// Achievement array which will hold data about the achievements and their state
Achievement_t g_Achievements[] =
{
	_ACH_ID(ACH_WIN_ONE_GAME, "Winner"),
	_ACH_ID(ACH_WIN_100_GAMES, "Champion"),
	_ACH_ID(ACH_TRAVEL_FAR_ACCUM, "Interstellar"),
	_ACH_ID(ACH_TRAVEL_FAR_SINGLE, "Orbiter"),
};

// Global access to Achievements object
CSteamAchievements* g_SteamAchievements = NULL;

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
	blueTankGO->AddComponent<dae::ScoreComponent>(std::make_unique<dae::ScoreComponent>(blueTankGO.get()));
	blueTankGO->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(blueTankGO.get(), 200.f, 300.f, 0.f));
	blueTankGO->AddComponent<dae::AchievementObserver>(std::make_unique<dae::AchievementObserver>(blueTankGO.get(), g_SteamAchievements));
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
	input.BindCommand(SDL_SCANCODE_Z, dae::KeyState::Up,
		std::make_unique<dae::ScoreCommand>(blueTankGO.get(), false));
	input.BindCommand(SDL_SCANCODE_X, dae::KeyState::Up,
		std::make_unique<dae::ScoreCommand>(blueTankGO.get(), true));

	// BlueTank health UI
	auto blueTankHPUI = std::make_unique<dae::GameObject>();
	blueTankHPUI->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(blueTankHPUI.get(), 0.f, 140.f));
	blueTankHPUI->AddComponent<dae::TextComponent>(std::make_unique<dae::TextComponent>(blueTankHPUI.get(), "Lives: 0", SDL_Color{ 0, 0, 255, 255 }, fontSmall));
	blueTankHPUI->AddComponent<dae::RenderComponent>(std::make_unique<dae::RenderComponent>(blueTankHPUI.get(), nullptr));
	blueTankHPUI->AddComponent<dae::HealthUIComponent>(std::make_unique<dae::HealthUIComponent>(blueTankHPUI.get(), blueTankGO->GetComponent<dae::HealthComponent>()));
	
	// BlueTank score UI
	auto blueTankScoreUI = std::make_unique<dae::GameObject>();
	blueTankScoreUI->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(blueTankScoreUI.get(), 0.f, 160.f));
	blueTankScoreUI->AddComponent<dae::TextComponent>(std::make_unique<dae::TextComponent>(blueTankScoreUI.get(), "Score: 0", SDL_Color{ 0, 0, 255, 255 }, fontSmall));
	blueTankScoreUI->AddComponent<dae::RenderComponent>(std::make_unique<dae::RenderComponent>(blueTankScoreUI.get(), nullptr));
	blueTankScoreUI->AddComponent<dae::ScoreUIComponent>(std::make_unique<dae::ScoreUIComponent>(blueTankScoreUI.get(), blueTankGO->GetComponent<dae::ScoreComponent>()));

	scene.Add(std::move(blueTankScoreUI));
	scene.Add(std::move(blueTankHPUI));
	scene.Add(std::move(blueTankGO));


	// Red Tank
	auto redTankGO = std::make_unique<dae::GameObject>();
	redTankGO->AddComponent<dae::HealthComponent>(std::make_unique<dae::HealthComponent>(redTankGO.get()));
	redTankGO->AddComponent<dae::ScoreComponent>(std::make_unique<dae::ScoreComponent>(redTankGO.get()));
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
	input.BindCommand(0, dae::ControllerButton::ButtonX, dae::KeyState::Up,
		std::make_unique<dae::DamageCommand>(redTankGO.get()));
	input.BindCommand(0, dae::ControllerButton::ButtonA, dae::KeyState::Up,
		std::make_unique<dae::ScoreCommand>(redTankGO.get(), true));
	input.BindCommand(0, dae::ControllerButton::ButtonB, dae::KeyState::Up,
		std::make_unique<dae::ScoreCommand>(redTankGO.get(), false));

	// redTankUI
	auto redTankHPUI = std::make_unique<dae::GameObject>();
	redTankHPUI->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(redTankHPUI.get(), 0.f, 180.f));
	redTankHPUI->AddComponent<dae::TextComponent>(std::make_unique<dae::TextComponent>(redTankHPUI.get(), "Lives: 0", SDL_Color{ 255, 0, 0, 255 }, fontSmall));
	redTankHPUI->AddComponent<dae::RenderComponent>(std::make_unique<dae::RenderComponent>(redTankHPUI.get(), nullptr));
	redTankHPUI->AddComponent<dae::HealthUIComponent>(std::make_unique<dae::HealthUIComponent>(redTankHPUI.get(), redTankGO->GetComponent<dae::HealthComponent>()));

	// RedTank score UI
	auto redTankScoreUI = std::make_unique<dae::GameObject>();
	redTankScoreUI->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(redTankScoreUI.get(), 0.f, 200.f));
	redTankScoreUI->AddComponent<dae::TextComponent>(std::make_unique<dae::TextComponent>(redTankScoreUI.get(), "Score: 0", SDL_Color{ 255, 0, 0, 255 }, fontSmall));
	redTankScoreUI->AddComponent<dae::RenderComponent>(std::make_unique<dae::RenderComponent>(redTankScoreUI.get(), nullptr));
	redTankScoreUI->AddComponent<dae::ScoreUIComponent>(std::make_unique<dae::ScoreUIComponent>(redTankScoreUI.get(), redTankGO->GetComponent<dae::ScoreComponent>()));

	scene.Add(std::move(redTankScoreUI));
	scene.Add(std::move(redTankHPUI));
	scene.Add(std::move(redTankGO));


	// Instructions GameObject controller
	auto instructionsRedGO = std::make_unique<dae::GameObject>();
	instructionsRedGO->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(instructionsRedGO.get(), 0.f, 80.f));
	instructionsRedGO->AddComponent<dae::TextComponent>(std::make_unique<dae::TextComponent>(instructionsRedGO.get(), "Use the D-Pad to move RedTank, X to inflict damage, A and B to get Kills", SDL_Color{ 255, 0, 0, 255 }, fontSmall));
	instructionsRedGO->AddComponent<dae::RenderComponent>(std::make_unique<dae::RenderComponent>(instructionsRedGO.get(), nullptr));

	scene.Add(std::move(instructionsRedGO));

	// Instructions GameObject keyboard
	auto instructionsBlueGO = std::make_unique<dae::GameObject>();
	instructionsBlueGO->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(instructionsBlueGO.get(), 0.f, 100.f));
	instructionsBlueGO->AddComponent<dae::TextComponent>(std::make_unique<dae::TextComponent>(instructionsBlueGO.get(), "Use WASD to move BlueTank, C to inflict damage, Z and X to get Kills", SDL_Color{ 0, 0, 255, 255 }, fontSmall));
	instructionsBlueGO->AddComponent<dae::RenderComponent>(std::make_unique<dae::RenderComponent>(instructionsBlueGO.get(), nullptr));

	scene.Add(std::move(instructionsBlueGO));

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

#if USE_STEAMWORKS
	g_SteamAchievements = new CSteamAchievements(g_Achievements, 4);
#endif // USE_STEAMWORKS

	engine.Run(load);

#if USE_STEAMWORKS
	if (g_SteamAchievements)
		delete g_SteamAchievements;
#endif // USE_STEAMWORKS

    return 0;
}
