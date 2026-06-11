#include "MenuStateComponent.h"
#include "StatesMenu.h"
#include "TextComponent.h"

tron::MenuStateComponent::MenuStateComponent(dae::GameObject* owner, dae::TextComponent* gameModeText, std::function<void(GameMode)> onStart, std::function<void()> onQuit)
    : Component(owner)
    , m_pGameModeText(gameModeText)
    , m_OnStart(std::move(onStart))
    , m_OnQuit(std::move(onQuit))
    , m_pState(std::make_unique<IdleMenuState>())
{
    m_pState->OnEnter(*this);
}

void tron::MenuStateComponent::Update(float deltaTime)
{
    auto newState = m_pState->Update(*this);
    if (newState)
    {
        m_pState->OnExit(*this);
        m_pState = std::move(newState);
        m_pState->OnEnter(*this);
    }
}

void tron::MenuStateComponent::Start()
{
    if (m_OnStart) m_OnStart(m_GameMode);
}

void tron::MenuStateComponent::Quit()
{
    if (m_OnQuit) 
        m_OnQuit();
}

void tron::MenuStateComponent::ToggleGameMode()
{
    switch (m_GameMode)
    {
    case GameMode::SinglePlayer: 
        m_GameMode = GameMode::CoOp;         
        break;
    case GameMode::CoOp:         
        m_GameMode = GameMode::Versus;       
        break;
    case GameMode::Versus:       
        m_GameMode = GameMode::SinglePlayer; 
        break;
    }
    UpdateGameModeText();
}

void tron::MenuStateComponent::UpdateGameModeText()
{
    if (m_pGameModeText)
        m_pGameModeText->SetText("Mode: " + GameModeToString(m_GameMode));
}

std::string tron::MenuStateComponent::GameModeToString(GameMode mode)
{
    switch (mode)
    {
    case GameMode::SinglePlayer: 
        return "Singleplayer";
    case GameMode::CoOp:         
        return "Co-op";
    case GameMode::Versus:       
        return "Versus";
    default:                     
        return "Unknown";
    }
}