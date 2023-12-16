#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle11Helpers
{
    using GalaxyGrid = Core::Math::Grid2d<char, int32>;
    using GalaxyCoordinate = GalaxyGrid::Grid2dCoordinate;

    constexpr char emptySpace{ '.' };
    constexpr char expandedSpace{ '*' };
    constexpr char galaxy{ '#' };

    struct Space
    {
        GalaxyGrid grid;
        std::vector<GalaxyCoordinate> galaxyLocations{ };
    };

    auto BuildSpace(const std::vector<std::string>& input)
    {
        auto isCharEmptySpace{ [](char c) {
            return c == emptySpace;
        } };

        Space space{
            .grid{ Core::Math::ConstructGrid(input) }
        };

        for (const auto& [coordinate, value] : space.grid)
        {
            if (value == galaxy)
            {
                space.galaxyLocations.emplace_back(coordinate);
            }
        }

        // Replace empty space rows with expanded space characters.
        for (auto row{ 0 }; row < input.size(); ++row)
        {
            const auto& line{ input[row] };
            if (std::all_of(line.begin(), line.end(), isCharEmptySpace))
            {
                for (auto col{ 0 }; col < space.grid.GetWidth(); ++col)
                {
                    space.grid.at({ col, row }).second = expandedSpace;
                }
            }
        }

        // Insert duplicate columns when the entire column is equal to empty space.
        for (int32 col{ static_cast<int32>(input[0].size()) - 1 }; col >= 0; --col)
        {
            // Check if the column is empty in every row.
            bool isEmptySpace{ true };
            for (int32 row{ 0 }; row < input.size(); ++row)
            {
                isEmptySpace &= isCharEmptySpace(input[row][col]);
            }

            // If the column was empty, replace it with expanded space.
            if (isEmptySpace)
            {
                for (int32 row{ 0 }; row < input.size(); ++row)
                {
                    space.grid.at({ col, row }).second = expandedSpace;
                }
            }
        }

        return space;
    }

    auto FindShortestPathBetween(const GalaxyGrid& grid, const GalaxyCoordinate& from, const GalaxyCoordinate& to, const uint64 expandedSpaceCost)
    {
        uint64 normalSpaceTraversed{ 0 };
        uint64 expandedSpaceTraversed{ 0 };

        auto accumulateTraversalCost{ [&normalSpaceTraversed, &expandedSpaceTraversed](char value) {
            if (value != expandedSpace) { ++normalSpaceTraversed;   }
            else                        { ++expandedSpaceTraversed; }
        } };

        for (auto x{ from.x }; x != to.x; x < to.x ? ++x : --x)
        {
            auto value{ grid.at({ x, from.y }).second };
            accumulateTraversalCost(value);
        }

        for (auto y{ from.y }; y != to.y; y < to.y ? ++y : --y)
        {
            auto value{ grid.at({ from.x, y }).second };
            accumulateTraversalCost(value);
        }

        return (normalSpaceTraversed) + (expandedSpaceTraversed * expandedSpaceCost);
    }

    auto FindShortestPathBetweenAllGalaxies(const Space& space, const uint64 expandedSpaceCost)
    {
        uint64 sumOfShortestPaths{ 0 };
        for (auto galaxyIndex{ 0 }; galaxyIndex < space.galaxyLocations.size() - 1; ++galaxyIndex)
        {
            auto galaxyCoordinate{ space.galaxyLocations[galaxyIndex] };
            for (auto otherGalaxyIndex{ galaxyIndex + 1 }; otherGalaxyIndex < space.galaxyLocations.size(); ++otherGalaxyIndex)
            {
                auto otherGalaxyCoordinate{ space.galaxyLocations[otherGalaxyIndex] };
                sumOfShortestPaths += FindShortestPathBetween(space.grid, galaxyCoordinate, otherGalaxyCoordinate, expandedSpaceCost);
            }
        }
        return sumOfShortestPaths;
    }
}

void Puzzle11::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle11Helpers;

    auto space{ BuildSpace(input) };
    std::cout << FindShortestPathBetweenAllGalaxies(space, 2) << std::endl;
}

void Puzzle11::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle11Helpers;
    auto space{ BuildSpace(input) };
    std::cout << FindShortestPathBetweenAllGalaxies(space, 1000000) << std::endl;
}
