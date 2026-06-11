#pragma once
#include "Component.h"
#include "MenuState.h"
#include "GameMode.h"
#include <memory>
#include <functional>

namespace dae { class TextComponent; }

namespace tron
{
    class MenuStateComponent final : public dae::Component
    {
    public:
        MenuStateComponent(dae::GameObject* owner, dae::TextComponent* gameModeText, std::function<void(GameMode)> onStart, std::function<void()> onQuit);

        void Update(float deltaTime) override;

        void Start();
        void Quit();
        void ToggleGameMode();
        void UpdateGameModeText();

        GameMode GetGameMode() const 
        { 
            return m_GameMode; 
        }

    private:
        std::unique_ptr<MenuState> m_pState;
        dae::TextComponent* m_pGameModeText;

        std::function<void(GameMode)> m_OnStart;
        std::function<void()> m_OnQuit;

        GameMode m_GameMode{ GameMode::SinglePlayer };

        static std::string GameModeToString(GameMode mode);
    };
}