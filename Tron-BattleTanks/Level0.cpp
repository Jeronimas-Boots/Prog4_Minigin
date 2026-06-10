#include "Level0.h"
#include "LevelLoader.h"
#include "LevelBuilder.h"

void tron::Level0::Load(dae::Scene& scene)
{
	auto grid = dae::LevelLoader::Load(GetCSVPath());
	tron::LevelBuilder::BuildScene(scene, grid);
}