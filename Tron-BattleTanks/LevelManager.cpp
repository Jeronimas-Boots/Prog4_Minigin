#include "LevelManager.h"
#include "SceneManager.h"
#include <stdexcept>
#include "Level0.h"

void tron::LevelManager::RegisterLevel(std::unique_ptr<Level> level)
{
    m_Levels.push_back(std::move(level));
}

void tron::LevelManager::LoadLevel(int index, tron::GameMode mode)
{
    assert(index >= 0 && index < static_cast<int>(m_Levels.size()));

    m_CurrentIndex = index;

    auto& sceneManager = dae::SceneManager::GetInstance();
    sceneManager.ClearScenes();
    auto& scene = sceneManager.CreateScene();

    m_Levels[m_CurrentIndex]->Load(scene, mode, this);
}

void tron::LevelManager::NextLevel()
{
    const int count = static_cast<int>(m_Levels.size());
    if (count == 0) return;

    int index = m_CurrentIndex;
    for (int i = 0; i < count; ++i)
    {
        index = (index + 1) % count;
        if (dynamic_cast<Level0*>(m_Levels[index].get()) != nullptr)
        {
            LoadLevel(index);
            return;
        }
    }
}

void tron::LevelManager::RestartLevel()
{
    LoadLevel(m_CurrentIndex);
}