#pragma once
#include <string>
#include "GameMode.h"

namespace dae { class Scene; }
namespace tron
{
	class LevelManager;
	class Level
	{
	public:
		virtual ~Level() = default;
		virtual void Load(dae::Scene& scene, GameMode mode, LevelManager* levelManager) = 0;
		virtual std::string GetCSVPath() const = 0;
	};
}