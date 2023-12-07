#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle03Helpers
{
    using Grid = Core::Math::Grid2d<char, int32>;

    struct Symbol
    {
        char glyph{ };
        std::vector<int32> neighboringNumbers{ };
    };

    constexpr char period{ '.' };
    constexpr char gear{ '*' };

    auto IsGear(char c) { return c == gear; }
    auto IsSymbol(char c) { return c != period && !std::isdigit(c); }

    auto GetAdjacentSymbolCoordinates(const Grid& grid, const Grid::Grid2dCoordinate& coordinate, std::unordered_set<Grid::Grid2dCoordinate>& adjacentSymbolCoordinates)
    {
        Core::Math::Range<int32> xBounds{ 0, grid.GetWidth() - 1 };
        Core::Math::Range<int32> yBounds{ 0, grid.GetHeight() - 1 };

        for (const auto& adjacentCoordinate : Grid::Grid2dCoordinate::cartesianNeighboringDirectionsWithDiagonals)
        {
            const auto neighbor{ coordinate + adjacentCoordinate };
            const bool bIsInBounds{ xBounds.ContainsInclusive(neighbor.x) && yBounds.ContainsInclusive(neighbor.y) };
            if (bIsInBounds && IsSymbol(grid.at(neighbor)))
            {
                adjacentSymbolCoordinates.emplace(neighbor);
            }
        }
    }

    std::unordered_map<Grid::Grid2dCoordinate, Symbol> GetSymbols(const Grid& grid)
    {
        std::unordered_map<Grid::Grid2dCoordinate, Symbol> symbols{ };

        // Run one pass to get all symbols and their coordinates.
        for (const auto& [coordinate, value] : grid)
        {
            if (IsSymbol(value))
            {
                symbols.emplace(coordinate, Symbol{ .glyph{ value } });
            }
        }

        // Run another pass to find all the numberes and add them to the neighboring symbols.
        for (auto iter{ grid.rbegin() }; iter != grid.rend(); ++iter)
        {
            const auto& [coordinate, value] { *iter };
            if (std::isdigit(value))
            {
                // Found start of digit.
                // Create the number and check surrounding characters for symbols.
                auto number{ 0 };
                auto digit{ 1 };
                std::unordered_set<Grid::Grid2dCoordinate> adjacentSymbolCoordinates{ };
                while (true)
                {
                    const auto& [runnerCoordinate, runnerValue] { *iter };
                    if (runnerCoordinate.y != coordinate.y || !std::isdigit(runnerValue)) { break; }

                    number += digit * std::stoi(std::string{ runnerValue });
                    digit *= 10;
                    GetAdjacentSymbolCoordinates(grid, runnerCoordinate, adjacentSymbolCoordinates);
                    ++iter;
                }

                for (const auto& adjacentSymbolCoordinate : adjacentSymbolCoordinates)
                {
                    symbols[adjacentSymbolCoordinate].neighboringNumbers.emplace_back(number);
                }
            }
        }

        return symbols;
    }
}

void Puzzle03::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle03Helpers;

    auto grid{ Core::Math::ConstructGrid(input) };
    auto symbols{ GetSymbols(grid) };

    auto sum{ 0 };
    for (const auto& [coordinate, symbol] : symbols)
    {
        for (auto& value : symbol.neighboringNumbers)
        {
            sum += value;
        }
    }
    std::cout << sum << std::endl;
}

void Puzzle03::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle03Helpers;

    auto grid{ Core::Math::ConstructGrid(input) };
    auto symbols{ GetSymbols(grid) };

    auto sum{ 0 };
    for (const auto& [coordinate, symbol] : symbols)
    {
        if (!IsGear(symbol.glyph)) { continue; }
        if (symbol.neighboringNumbers.size() == 2)
        {
            sum += static_cast<int64>(symbol.neighboringNumbers[0]) * static_cast<int64>(symbol.neighboringNumbers[1]);
        }
    }
    std::cout << sum << std::endl;
}
