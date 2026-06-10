#pragma once
#include <string>
#include <vector>

namespace dae
{
    class LevelLoader
    {
    public:
        // Returns a row-major grid of tile IDs, or empty on failure
        static std::vector<std::vector<int>> Load(const std::string& filepath);
    };
}