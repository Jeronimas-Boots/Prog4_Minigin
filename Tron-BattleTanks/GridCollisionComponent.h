#pragma once
#include "Component.h"
#include <vector>

namespace tron
{
    class GridCollisionComponent final : public dae::Component
    {
    public:
        GridCollisionComponent(dae::GameObject* owner,
            const std::vector<std::vector<int>>& grid,
            float tileSize, float offsetX, float offsetY);

        bool IsWalkable(float worldX, float worldY) const;
        bool IsWall(float worldX, float worldY) const;

        float GetTileSize()  const { return m_TileSize; }
        float GetOffsetX()   const { return m_OffsetX; }
        float GetOffsetY()   const { return m_OffsetY; }

    private:
        const std::vector<std::vector<int>> m_Grid;
        float m_TileSize;
        float m_OffsetX;
        float m_OffsetY;

        // Tile values that block movement
        static bool IsWalkableTile(int tileValue);
    };
}