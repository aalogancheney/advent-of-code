#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle14Helpers
{
    using CaveGrid2d = Core::Math::Grid2d<char>;

    constexpr char air{ '.' };
    constexpr char wall{ '#' };
    constexpr char sand{ 'o' };

    CaveGrid2d CreateCaveGrid(const std::vector<std::string>& input, bool bCreateFloor = false)
    {
        CaveGrid2d grid{ 1000, 1000 };
        for (size_t x{ 0 }; x < 1000; ++x)
        {
            for (size_t y{ 0 }; y < 1000; ++y)
            {
                grid.at(x, y) = air;
            }
        }

        size_t largestY{ 0 };
        for (const auto& line : input)
        {
            std::vector<Grid2dCoordinate> vectors{ };
            std::vector<std::string> coordinates{ Core::SplitString(line, " -> ") };
            for (const auto& coordinate : coordinates)
            {
                std::vector<std::string> vectorStrings{ Core::SplitString(coordinate, ",") };
                check(vectorStrings.size() == 2);
                Grid2dCoordinate location{ static_cast<size_t>(std::stoll(vectorStrings[0])), static_cast<size_t>(std::stoll(vectorStrings[1])) };
                vectors.emplace_back(location);
                if (location.y >= largestY)
                {
                    largestY = location.y;
                }
            }

            for (size_t index{ 0 }; index < vectors.size() - 1; ++index)
            {
                Grid2dCoordinate start{ vectors[index] };
                Grid2dCoordinate end{ vectors[index + 1] };
                if (start.x == end.x)
                {
                    if (start.y < end.y)
                    {
                        for (size_t y{ start.y }; y <= end.y; ++y)
                        {
                            grid.at(start.x, y) = wall;
                        }
                    }
                    else
                    {
                        for (size_t y{ start.y }; y >= end.y; --y)
                        {
                            grid.at(start.x, y) = wall;
                        }
                    }
                }
                else
                {
                    if (start.x < end.x)
                    {
                        for (size_t x{ start.x }; x <= end.x; ++x)
                        {
                            grid.at(x, start.y) = wall;
                        }
                    }
                    else
                    {
                        for (size_t x{ start.x }; x >= end.x; --x)
                        {
                            grid.at(x, start.y) = wall;
                        }
                    }
                }
            }
        }

        if (bCreateFloor)
        {
            for (size_t x{ 0 }; x < 1000; ++x)
            {
                grid.at(x, largestY + 2) = wall;
            }
        }

        return grid;
    }

    bool IsBlocked(char c) { return c == wall || c == sand; }

    int32 SimulateSandFalling(CaveGrid2d& caveGrid, Grid2dCoordinate startPosition)
    {
        int32 sandGrainsAtRest{ 0 };
        
        bool isOffGrid{ false };
        while (!isOffGrid)
        {
            Grid2dCoordinate position{ startPosition };
            if (caveGrid.at(position) == sand) { break; }
            caveGrid.at(position) = sand;
            bool isAtRest{ false };
            while (!isAtRest)
            {
                Grid2dCoordinate down{ position + Grid2dCoordinate::up };
                if (down.y >= 1000)
                { 
                    isOffGrid = true;
                    break;
                }
                Grid2dCoordinate downLeft{ down + Grid2dCoordinate::left };
                Grid2dCoordinate downRight{ down + Grid2dCoordinate::right };
                if (!IsBlocked(caveGrid.at(down)))
                {
                    caveGrid.at(down) = sand;
                    caveGrid.at(position) = air;
                    position = down;
                }
                else if (!IsBlocked(caveGrid.at(downLeft)))
                {
                    caveGrid.at(downLeft) = sand;
                    caveGrid.at(position) = air;
                    position = downLeft;
                }
                else if (!IsBlocked(caveGrid.at(downRight)))
                {
                    caveGrid.at(downRight) = sand;
                    caveGrid.at(position) = air;
                    position = downRight;
                }
                else
                {
                    ++sandGrainsAtRest;
                    isAtRest = true;
                }
            }
        }
        
        return sandGrainsAtRest;
    }
}

void Puzzle14::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle14Helpers;

    CaveGrid2d caveGrid{ CreateCaveGrid(input) };
    int32 grainsAtRest{ SimulateSandFalling(caveGrid, Grid2dCoordinate{500, 0}) };
    std::cout << grainsAtRest << std::endl;
}

void Puzzle14::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle14Helpers;

    CaveGrid2d caveGrid{ CreateCaveGrid(input, true) };
    int32 grainsAtRest{ SimulateSandFalling(caveGrid, Grid2dCoordinate{500, 0}) };
    std::cout << grainsAtRest << std::endl;
}
