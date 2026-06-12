#pragma once
#include "SoundSystem.h"
#include <memory>

namespace dae
{
    class SDL_MixerSoundSystem final : public SoundSystem
    {
    public:
        SDL_MixerSoundSystem();
        ~SDL_MixerSoundSystem() override;

        SDL_MixerSoundSystem(const SDL_MixerSoundSystem&) = delete;
        SDL_MixerSoundSystem(SDL_MixerSoundSystem&&) = delete;
        SDL_MixerSoundSystem& operator=(const SDL_MixerSoundSystem&) = delete;
        SDL_MixerSoundSystem& operator=(SDL_MixerSoundSystem&&) = delete;

        void Play(const std::string& filepath, float volume, int loops = 0) override;
        void PlayMusic(const std::string& filepath, float volume, int loops = -1) override;
        void Update() override;

    private:
        class SoundSystemImpl;
        std::unique_ptr<SoundSystemImpl> m_pImpl;
    };
}