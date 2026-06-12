#pragma once 
#include <memory>
#include "SoundSystem.h"

namespace dae
{
	class ServiceLocator final
	{
		static std::unique_ptr<SoundSystem> m_SsInstance;

	public:
		static SoundSystem& GetSoundSystem();

		static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss);
	};
}