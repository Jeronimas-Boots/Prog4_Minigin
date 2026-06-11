#pragma once	
#include "Level.h"
#include "LevelBuilder.h"

namespace tron
{
	class Level0 final : public Level
	{
	public:
		void Load(dae::Scene& scene, GameMode mode = GameMode::SinglePlayer) override;
		std::string GetCSVPath() const override { return "Level/LevelLayout0.csv"; }

	private:
	};
}