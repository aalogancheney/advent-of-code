#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle03Helpers
{
    constexpr char period{ '.' };
    constexpr char gear{ '*' };

    static std::vector<Vector2d32> adjacentCoordinates
    {
        Vector2d32{  0, -1 }, // Left
        Vector2d32{ -1, -1 }, // Top-left
        Vector2d32{ -1,  0 }, // Top
        Vector2d32{ -1,  1 }, // Top-right
        Vector2d32{  0,  1 }, // Right
        Vector2d32{  1,  1 }, // Bottom-right
        Vector2d32{  1,  0 }, // Bottom
        Vector2d32{  1, -1 }, // Bottom-left
    };

    static auto IsGear(char c) { return c == gear; }
    static auto IsSymbol(char c) { return c != period && !std::isdigit(c); }

    static auto IsCoordinateAdjacentToSymbol(const std::vector<std::string>& input, int32 x, int32 y)
    {
        static Core::Math::Range<int32> xBounds{ 0, static_cast<int32>(input.size() - 1) };
        static Core::Math::Range<int32> yBounds{ 0, static_cast<int32>(input[0].size() - 1) };
        bool bIsNeighboringSymbol{ false };
        for (const auto& adjacentCoordinate : adjacentCoordinates)
        {
            Vector2d32 neighbor{ x + adjacentCoordinate.x, y + adjacentCoordinate.y };
            const bool bIsInBounds{ xBounds.IsInRange(neighbor.x) && yBounds.IsInRange(neighbor.y) };
            bIsNeighboringSymbol |= bIsInBounds && IsSymbol(input[neighbor.x][neighbor.y]);
        }
        return bIsNeighboringSymbol;
    }

    static auto GetAdjacentGears(const std::vector<std::string>& input, int32 x, int32 y, std::unordered_set<Vector2d32>& adjacentGears)
    {
        static Core::Math::Range<int32> xBounds{ 0, static_cast<int32>(input.size() - 1) };
        static Core::Math::Range<int32> yBounds{ 0, static_cast<int32>(input[0].size() - 1) };
        bool bIsNeighboringSymbol{ false };
        for (const auto& adjacentCoordinate : adjacentCoordinates)
        {
            Vector2d32 neighbor{ x + adjacentCoordinate.x, y + adjacentCoordinate.y };
            const bool bIsInBounds{ xBounds.IsInRange(neighbor.x) && yBounds.IsInRange(neighbor.y) };
            if (bIsInBounds && IsGear(input[neighbor.x][neighbor.y]))
            {
                adjacentGears.emplace(neighbor);
            }
        }
    }
}

void Puzzle03::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle03Helpers;

    auto sum{ 0 };

    // Traverse each row top-to-bottom, and each column from right-to-left. This helps
    // build numbers in-place.
    for (int32 row{ 0 }; row < input.size(); ++row)
    {
        for (int32 col{ static_cast<int32>(input[row].length() - 1) }; col >= 0; --col)
        {
            if (std::isdigit(input[row][col]))
            {
                // Found start of digit. Create the number and check surrounding characters for
                // symbols (i.e. non-digit, non-period characters) to see if the resulting number
                // should be counted for the result.
                bool bIsNumberAdjacentToSymbol{ false };
                int32 number{ 0 };
                int32 digit{ 1 };
                while (std::isdigit(input[row][col]))
                {
                    number += digit * std::stoi(std::string{ input[row][col] });
                    digit *= 10;
                    bIsNumberAdjacentToSymbol |= IsCoordinateAdjacentToSymbol(input, row, col--);
                }

                if (bIsNumberAdjacentToSymbol)
                {
                    sum += number;
                }
            }
        }
    }

    std::cout << sum << std::endl;
}

void Puzzle03::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle03Helpers;

    // Run one pass on grid to find all gears.
    std::unordered_map<Vector2d32, std::vector<int32>> gears{ };
    for (int32 row{ 0 }; row < input.size(); ++row)
    {
        for (int32 col{ static_cast<int32>(input[row].length() - 1) }; col >= 0; --col)
        {
            if (IsGear(input[row][col])) { gears[Vector2d32{ row, col }] = { }; }
        }
    }

    // Traverse each row top-to-bottom, and each column from right-to-left. This helps
    // build numbers in-place.
    for (int32 row{ 0 }; row < input.size(); ++row)
    {
        for (int32 col{ static_cast<int32>(input[row].length() - 1) }; col >= 0; --col)
        {
            if (std::isdigit(input[row][col]))
            {
                // Found start of digit. Create the number and add the resulting number to
                // the set of all adjacent gears.
                std::unordered_set<Vector2d32> neighboringGears{ };
                int32 number{ 0 };
                int32 digit{ 1 };
                while (std::isdigit(input[row][col]))
                {
                    number += digit * std::stoi(std::string{ input[row][col] });
                    digit *= 10;
                    GetAdjacentGears(input, row, col--, neighboringGears);
                }

                for (const auto& gear : neighboringGears)
                {
                    gears[gear].emplace_back(number);
                }
            }
        }
    }

    int64 sum{ 0 };
    for (const auto& [coordinate, neighboringNumbers] : gears)
    {
        if (neighboringNumbers.size() == 2)
        {
            sum += static_cast<int64>(neighboringNumbers[0]) * static_cast<int64>(neighboringNumbers[1]);
        }
    }
    std::cout << sum << std::endl;
}
