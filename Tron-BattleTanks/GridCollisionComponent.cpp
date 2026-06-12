#include "GridCollisionComponent.h"

tron::GridCollisionComponent::GridCollisionComponent(dae::GameObject* owner,
    const std::vector<std::vector<int>>& grid,
    float tileSize, float offsetX, float offsetY)
    : Component(owner)
    , m_Grid(grid)
    , m_TileSize(tileSize)
    , m_OffsetX(offsetX)
    , m_OffsetY(offsetY)
{
}

bool tron::GridCollisionComponent::IsWalkable(float worldX, float worldY) const
{
    // Convert world position to grid coordinates
    const int col = static_cast<int>((worldX - m_OffsetX) / m_TileSize);
    const int row = static_cast<int>((worldY - m_OffsetY) / m_TileSize);

    // Out of bounds = not walkable
    if (row < 0 || row >= static_cast<int>(m_Grid.size()))    
        return false;
    if (col < 0 || col >= static_cast<int>(m_Grid[row].size())) 
        return false;

    return IsWalkableTile(m_Grid[row][col]);
}

bool tron::GridCollisionComponent::IsWall(float worldX, float worldY) const
{
    const int col = static_cast<int>((worldX - m_OffsetX) / m_TileSize);
    const int row = static_cast<int>((worldY - m_OffsetY) / m_TileSize);

    if (row < 0 || row >= static_cast<int>(m_Grid.size()))    return false;
    if (col < 0 || col >= static_cast<int>(m_Grid[row].size())) return false;

    return m_Grid[row][col] == 0;
}

bool tron::GridCollisionComponent::IsWalkableTile(int tileValue)
{
    switch (tileValue)
    {
    case 0: // Wall
    case 1: // Void
        return false;
    case 2: // Path
    case 3: // Teleport
    case 4: // EnemySpawn
    case 6: // PlayerSpawn
        return true;
    default:
        return false;
    }
}