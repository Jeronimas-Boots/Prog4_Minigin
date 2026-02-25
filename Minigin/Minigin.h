#pragma once
#include <string>
#include <functional>
#include <filesystem>

namespace dae
{
	class Minigin final
	{
	private:
		bool m_quit{};
		std::chrono::steady_clock::time_point m_LastTime{};
		float m_Lag = 0.f;
		const float m_FixedTimeStep = 1.f / 30.f;	// in seconds
		const float m_MsPerFrame = 1000.f / 144.f;	// in miliseconds

	public:
		explicit Minigin(const std::filesystem::path& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);
		void RunOneFrame();

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
	};
}