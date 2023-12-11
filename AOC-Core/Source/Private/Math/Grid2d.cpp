#include "pch.h"
#include "Core.h"
#include "Math/Grid2d.h"

namespace Core::Math
{
    Grid2d<char, int32> ConstructGrid(const std::vector<std::string>& input)
    {
        check(input.size() > 0);
        check(input[0].size() > 0);
        Grid2d<char, int32> grid{ static_cast<int32>(input[0].size()), static_cast<int32>(input.size()) };
        for (auto& [coordinate, value] : grid)
        {
            // Grid iterator traverses in row-major order. Grab input in the correct order.
            grid.at(coordinate).second = input[coordinate.y][coordinate.x];
        }
        return grid;
    }
}
