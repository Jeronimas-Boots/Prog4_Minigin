#pragma once
#include "Command.h"
#include "LevelManager.h"

namespace tron
{
    class NextLevelCommand final : public dae::Command
    {
    public:
        explicit NextLevelCommand(LevelManager* levelManager)
            : m_LevelManager(levelManager) {
        }

        void Execute(float) override
        {
            m_LevelManager->NextLevel();
        }

    private:
        LevelManager* m_LevelManager;
    };
}