#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle09Helpers
{
    using namespace Core;
    using namespace Core::Math;

    void UpdateHeadPosition(const char direction, Vector2_32& headPosition)
    {
        switch (direction)
        {
        case 'U':
            headPosition += Vector2_32::up;
            break;
        case 'D':
            headPosition += Vector2_32::down;
            break;
        case 'L':
            headPosition += Vector2_32::left;
            break;
        case 'R':
            headPosition += Vector2_32::right;
            break;
        }
    }

    void UpdateTailPosition(const Vector2_32& headPosition, Vector2_32& tailPosition)
    {
        auto tailToHead{ headPosition - tailPosition };
        auto length{ tailToHead.Length() };

        if (length <= 1)
        {
            return;
        }
        
        check(length == 2);
        tailToHead.x = (tailToHead.x > 0) - (tailToHead.x < 0);
        tailToHead.y = (tailToHead.y > 0) - (tailToHead.y < 0);
        tailPosition += tailToHead;
    }
}

void Puzzle09::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle09Helpers;

    Vector2_32 headPosition{ Vector2_32::zero };
    Vector2_32 tailPosition{ headPosition };

    std::unordered_set<Vector2_32> visitedPositions{ tailPosition };

    for (const auto& command : input)
    {
        auto direction{ command[0] };
        auto moves{ std::stoi(command.substr(2)) };

        while (moves-- > 0)
        {
            UpdateHeadPosition(direction, headPosition);
            UpdateTailPosition(headPosition, tailPosition);
            visitedPositions.emplace(tailPosition);
        }
    }

    std::cout << visitedPositions.size() << std::endl;
}

void Puzzle09::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle09Helpers;

    auto positions{ std::vector<Vector2_32>{ 10 } };
    Vector2_32& headPosition{ positions[0] };
    Vector2_32& tailPosition{ positions[9] };
    std::unordered_set<Vector2_32> visitedPositions{ tailPosition };

    for (const auto& command : input)
    {
        auto direction{ command[0] };
        auto moves{ std::stoi(command.substr(2)) };

        while (moves-- > 0)
        {
            UpdateHeadPosition(direction, headPosition);

            // Update the position of every knot after the head moves.
            for (auto index{ 0 }, next{ 1 }; next < 10; ++index, ++next)
            {
                UpdateTailPosition(positions[index], positions[next]);
            }
            visitedPositions.emplace(tailPosition);
        }
    }

    std::cout << visitedPositions.size() << std::endl;
}
