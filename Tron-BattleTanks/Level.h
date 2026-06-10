#pragma once
#include <string>

namespace dae { class Scene; }
namespace tron
{
	class Level
	{
	public:
		virtual ~Level() = default;
		virtual void Load(dae::Scene& scene) = 0;
		virtual std::string GetCSVPath() const = 0;
	};
}