#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include <memory>

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	// Background GameObject
	auto backgroundGO = std::make_shared<dae::GameObject>();
	backgroundGO->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(backgroundGO.get(), 0.f, 0.f, 0.f));
	backgroundGO->AddComponent<dae::RenderComponent>(std::make_unique<dae::RenderComponent>(
		backgroundGO.get(),
		dae::ResourceManager::GetInstance().LoadTexture("background.png")));
	scene.Add(backgroundGO);

	// FPS GameObject
	auto fontSmall = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 26);
	auto fpsGO = std::make_shared<dae::GameObject>();
	fpsGO->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(fpsGO.get(), 10.f, 10.f, 0.f));
	fpsGO->AddComponent<dae::TextComponent>(std::make_unique<dae::TextComponent>(fpsGO.get(), "FPS: 0", SDL_Color{ 255, 255, 255, 255 }, fontSmall));
	fpsGO->AddComponent<dae::FPSComponent>(std::make_unique<dae::FPSComponent>(fpsGO.get()));
	scene.Add(fpsGO);

	// Logo GameObject
	auto logoGO = std::make_shared<dae::GameObject>();
	logoGO->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(logoGO.get(), 358.f, 180.f, 0.f));
	logoGO->AddComponent<dae::RenderComponent>(std::make_unique<dae::RenderComponent>(
		logoGO.get(),
		dae::ResourceManager::GetInstance().LoadTexture("logo.png")));
	scene.Add(logoGO);

	// Text GameObject
	auto fontLarge = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto textGo = std::make_shared<dae::GameObject>();
	textGo->AddComponent<dae::TransformComponent>(std::make_unique<dae::TransformComponent>(textGo.get(), 292.f, 20.f, 0.f));
	textGo->AddComponent<dae::TextComponent>(std::make_unique<dae::TextComponent>(textGo.get(), "Programming 4 Assignment", SDL_Color{ 255, 255, 0, 255 }, fontLarge));
	scene.Add(textGo);
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
