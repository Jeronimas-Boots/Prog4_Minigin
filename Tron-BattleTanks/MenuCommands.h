#pragma once
#include "Command.h"
#include "MenuStateComponent.h"

namespace tron
{
    class StartCommand final : public dae::Command
    {
    public:
        explicit StartCommand(MenuStateComponent* menu) 
            : m_Menu(menu) 
        {}
        void Execute(float) override 
        { 
            m_Menu->Start(); 
        }
    private:
        MenuStateComponent* m_Menu;
    };

    class QuitCommand final : public dae::Command
    {
    public:
        explicit QuitCommand(MenuStateComponent* menu) 
            : m_Menu(menu) 
        {}
        void Execute(float) override 
        { 
            m_Menu->Quit(); 
        }
    private:
        MenuStateComponent* m_Menu;
    };

    class ToggleGameModeCommand final : public dae::Command
    {
    public:
        explicit ToggleGameModeCommand(MenuStateComponent* menu) 
            : m_Menu(menu) 
        {}
        void Execute(float) override 
        { 
            m_Menu->ToggleGameMode(); 
        }
    private:
        MenuStateComponent* m_Menu;
    };
}