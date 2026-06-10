#include "Level0.h"
#include "LevelLoader.h"
#include "LevelBuilder.h"
#include "ResourceManager.h"

void tron::Level0::Load(dae::Scene& scene)
{
	auto grid = dae::ResourceManager::GetInstance().LoadCSV(GetCSVPath());
	tron::LevelBuilder::BuildScene(scene, grid);
}