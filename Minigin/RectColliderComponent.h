#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <string>

namespace dae
{
    struct Rect { float x, y, w, h; };

    inline bool RectsOverlap(const Rect& a, const Rect& b)
    {
        return a.x < b.x + b.w && a.x + a.w > b.x &&
            a.y < b.y + b.h && a.y + a.h > b.y;
    }

    class RectColliderComponent final : public Component
    {
    public:
        // Pass 0,0 to auto-size from RenderComponent
        RectColliderComponent(GameObject* owner, float width = 0.f, float height = 0.f, std::string tag = "");

        Rect GetWorldRect() const;
        const std::string& GetTag() const { return m_Tag; }

    private:
        float m_Width;
        float m_Height;
        std::string m_Tag;
    };
}