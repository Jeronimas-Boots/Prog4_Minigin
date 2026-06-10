#include "LevelManager.h"
#include "SceneManager.h"
#include <stdexcept>

void tron::LevelManager::RegisterLevel(std::unique_ptr<Level> level)
{
    m_Levels.push_back(std::move(level));
}

void tron::LevelManager::LoadLevel(int index)
{
    assert(index >= 0 && index < static_cast<int>(m_Levels.size()) && "LevelManager: invalid level index");

    m_CurrentIndex = index;

    auto& sceneManager = dae::SceneManager::GetInstance();
    sceneManager.ClearScenes();
    auto& scene = sceneManager.CreateScene();

    m_Levels[m_CurrentIndex]->Load(scene);
}

void tron::LevelManager::NextLevel()
{
    LoadLevel((m_CurrentIndex + 1) % static_cast<int>(m_Levels.size()));
}

void tron::LevelManager::RestartLevel()
{
    LoadLevel(m_CurrentIndex);
}