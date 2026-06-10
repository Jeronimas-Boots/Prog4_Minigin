#pragma once
#include <vector>
#include <memory>
#include "Level.h"

namespace tron
{
	class LevelManager
	{
    public:
        LevelManager() = default;
        ~LevelManager() = default;

        // Not copyable or movable
        LevelManager(const LevelManager&) = delete;
        LevelManager& operator=(const LevelManager&) = delete;
        LevelManager(LevelManager&&) = delete;
        LevelManager& operator=(LevelManager&&) = delete;

        void RegisterLevel(std::unique_ptr<Level> level);

        void LoadLevel(int index);
        void NextLevel();
        void RestartLevel();

        int GetCurrentIndex() const { return m_CurrentIndex; }
        int GetLevelCount() const { return static_cast<int>(m_Levels.size()); }

    private:
        std::vector<std::unique_ptr<Level>> m_Levels;
        int m_CurrentIndex{ -1 };
	};
}