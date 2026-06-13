#pragma once	
#include "Level.h"
#include "LevelBuilder.h"
#include <string>
namespace tron
{
	class LevelManager;
	class Level0 final : public Level
	{
	public:
		Level0(const std::string& levelPath);
		void Load(dae::Scene& scene, GameMode mode, LevelManager* levelManager) override;
		std::string GetCSVPath() const override { return m_LevelPath; }

	private:
		const std::string m_LevelPath;
	};
}