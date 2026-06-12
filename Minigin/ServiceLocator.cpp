#include "ServiceLocator.h"

std::unique_ptr<dae::SoundSystem> dae::ServiceLocator::m_SsInstance = std::make_unique<dae::NullSoundSystem>();


dae::SoundSystem& dae::ServiceLocator::GetSoundSystem()
{
	return *m_SsInstance;
}

void dae::ServiceLocator::RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss)
{
	m_SsInstance = ss == nullptr ? std::make_unique<NullSoundSystem>() : std::move(ss);
}
