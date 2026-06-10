// LevelBuilder.cpp
#include "LevelBuilder.h"
#include "Scene.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "ResourceManager.h"

tron::LevelLayout tron::LevelBuilder::BuildScene(dae::Scene& scene,
    const std::vector<std::vector<int>>& grid)
{
    constexpr float windowWidth = 1024.f;
    constexpr float windowHeight = 576.f;

    const int cols = static_cast<int>(grid[0].size());
    const int rows = static_cast<int>(grid.size());

    const float levelWidth = cols * TILE_SIZE;
    const float levelHeight = rows * TILE_SIZE;

    const float scale = std::min(windowWidth / levelWidth, windowHeight / levelHeight);

    const float offsetX = (windowWidth - levelWidth * scale) / 2.f;
    const float offsetY = (windowHeight - levelHeight * scale) / 2.f;

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            const float x = offsetX + col * TILE_SIZE * scale;
            const float y = offsetY + row * TILE_SIZE * scale;
            const TileType tile = static_cast<TileType>(grid[row][col]);

            switch (tile)
            {
            case TileType::Void:        
                SpawnTile(scene, x, y, scale, "Level/void.png");    
                break;
            case TileType::Wall:        
                SpawnTile(scene, x, y, scale, "Level/wall.png");    
                break;
            case TileType::Path:        
                SpawnTile(scene, x, y, scale, "Level/path.png");    
                break;
            case TileType::Teleport:    
                SpawnTile(scene, x, y, scale, "Level/teleport.png");
                break;
            case TileType::EnemySpawn:  
                SpawnTile(scene, x, y, scale, "Level/path.png");    
                break;
            case TileType::PlayerSpawn: 
                SpawnTile(scene, x, y, scale, "Level/path.png");    
                break;
            default:                    
                break;
            }
        }
    }
    return { grid, TILE_SIZE * scale, offsetX, offsetY, scale };
}

void tron::LevelBuilder::SpawnTile(dae::Scene& scene, float x, float y, float scale, const std::string& texturePath)
{
    auto go = std::make_unique<dae::GameObject>();
    go->AddComponent<dae::TransformComponent>(
        std::make_unique<dae::TransformComponent>(go.get(), x, y));

    auto* render = go->AddComponent<dae::RenderComponent>(
        std::make_unique<dae::RenderComponent>(
            go.get(),
            dae::ResourceManager::GetInstance().LoadTexture(texturePath)));

    render->SetScale(scale, scale);
    scene.Add(std::move(go));
}