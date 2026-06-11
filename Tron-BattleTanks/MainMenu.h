#pragma once
#include "Level.h"
#include "GameMode.h"
#include <functional>

namespace tron
{
    class MainMenu final : public Level
    {
    public:
        MainMenu(std::function<void(GameMode)> onStart, std::function<void()> onQuit);

        void Load(dae::Scene& scene, GameMode mode = GameMode::SinglePlayer) override;
        std::string GetCSVPath() const override 
        { 
            return ""; 
        }

    private:
        std::function<void(GameMode)> m_OnStart;
        std::function<void()> m_OnQuit;
    };
}