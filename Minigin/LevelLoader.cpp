#include "LevelLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<std::vector<int>> dae::LevelLoader::Load(const std::string& filepath)
{
    std::vector<std::vector<int>> grid;
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "LevelLoader: failed to open " << filepath << "\n";
        return grid;
    }

    std::string line;
    while (std::getline(file, line))
    {
        // Strip Windows \r if present
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (line.empty()) continue;

        std::vector<int> row;
        std::stringstream ss(line);
        std::string token;
        while (std::getline(ss, token, ','))
            row.push_back(std::stoi(token));

        grid.push_back(std::move(row));
    }
    return grid;
}