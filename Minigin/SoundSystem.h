#pragma once
#include <string>

namespace dae
{
	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;

		virtual void Play(const std::string& filepath, float volume, int loops = 0) = 0;
		virtual void PlayMusic(const std::string& filepath, float volume, int loops = -1) = 0;
		virtual void Update() = 0;
	};

	class NullSoundSystem final : public SoundSystem
	{
	public:
		void Play(const std::string&, float, int) override {}
		void PlayMusic(const std::string&, float, int) override {}
		void Update() override {}
	};

}