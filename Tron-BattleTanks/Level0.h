#pragma once	
#include "Level.h"

namespace tron
{
	class Level0 final : public Level
	{
	public:
		void Load(dae::Scene& scene) override;
		std::string GetCSVPath() const override { return "Level/LevelLayout0.csv"; }

	};
}