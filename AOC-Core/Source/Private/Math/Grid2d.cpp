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
        for (auto row{ 0 }; row < input.size(); ++row)
        {
            for (auto col{ 0 }; col < input[row].size(); ++col)
            {
                grid.at(col, row) = input[row][col];
            }
        }
        return grid;
    }
}
