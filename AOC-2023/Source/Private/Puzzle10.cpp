#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle10Helpers
{
    using PipeGrid = Core::Math::NavGrid2d<char, int32>;
    using PipeGridElement = PipeGrid::NavGrid2dElement;
    using PipeGridCoordinate = PipeGrid::NavGrid2dCoordinate;

    struct HotSprings
    {
        PipeGrid pipeGrid;
        PipeGridCoordinate startCoordinate;
    };

    bool IsNeighbor(const PipeGridElement& element, const PipeGridElement& neighbor)
    {
        static std::unordered_map<char, std::unordered_set<char>> validLeftNeighbors
        {
            { 'S', { '-', 'L', 'F' } },
            { '-', { '-', 'L', 'F' } },
            { 'J', { '-', 'L', 'F' } },
            { '7', { '-', 'L', 'F' } },
        };

        static std::unordered_map<char, std::unordered_set<char>> validRightNeighbors
        {
            { 'S', { '-', 'J', '7' } },
            { '-', { '-', 'J', '7' } },
            { 'L', { '-', 'J', '7' } },
            { 'F', { '-', 'J', '7' } },
        };

        static std::unordered_map<char, std::unordered_set<char>> validUpNeighbors
        {
            { 'S', { '|', '7', 'F' } },
            { '|', { '|', '7', 'F' } },
            { 'L', { '|', '7', 'F' } },
            { 'J', { '|', '7', 'F' } },
        };

        static std::unordered_map<char, std::unordered_set<char>> validDownNeighbors
        {
            { 'S', { '|', 'L', 'J' } },
            { '|', { '|', 'L', 'J' } },
            { '7', { '|', 'L', 'J' } },
            { 'F', { '|', 'L', 'J' } },
        };

        const auto& [elementCoordinate, elementNode] { element };
        const auto& [neighborCoordinate, neighborNode] { neighbor };

        // Up is down, and down is up in a grid that starts at 0 in the top row and increments on the way down.
        // TODO: Fix this by building the grid from the bottom up so that the cardinal directions make sense.
        const bool bIsNeighborAbove{ neighborCoordinate == elementCoordinate + PipeGridCoordinate::down
                && validUpNeighbors.contains(elementNode.value)
                && validUpNeighbors[elementNode.value].contains(neighborNode.value) };
        const bool bIsNeighborBelow{ neighborCoordinate == elementCoordinate + PipeGridCoordinate::up
                && validDownNeighbors.contains(elementNode.value)
                && validDownNeighbors[elementNode.value].contains(neighborNode.value) };
        const bool bIsNeighborLeft{ neighborCoordinate == elementCoordinate + PipeGridCoordinate::left
                && validLeftNeighbors.contains(elementNode.value)
                && validLeftNeighbors[elementNode.value].contains(neighborNode.value) };
        const bool bIsNeighborRight{ neighborCoordinate == elementCoordinate + PipeGridCoordinate::right
                && validRightNeighbors.contains(elementNode.value)
                && validRightNeighbors[elementNode.value].contains(neighborNode.value) };

        return bIsNeighborAbove || bIsNeighborBelow || bIsNeighborLeft || bIsNeighborRight;
    }

    auto CreateHotSprings(const std::vector<std::string>& input)
    {
        PipeGridCoordinate start{ };
        auto assignGridValues{ [&input, &start](PipeGridElement& element) {
            char value{ input[element.first.y][element.first.x] };
            element.second.value = value;
            if (value == 'S')
            {
                start = element.first;
            }
        } };
        HotSprings hotSprings{
            .pipeGrid{ static_cast<int32>(input[0].size()), static_cast<int32>(input.size()), assignGridValues, &IsNeighbor },
            .startCoordinate{ start }
        };
        return hotSprings;
    }
}

void Puzzle10::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle10Helpers;

    auto hotSprings{ CreateHotSprings(input) };

    // Since the start node has exactly 2 neighbors (guaranteed), set the start/end of the search
    // equal to either of those neighbors. The adjacency list doesn't add 'S' to any pipe pieces,
    // so the search will traverse the whole tunnel.
    auto& [coordinate, node] { hotSprings.pipeGrid.at(hotSprings.startCoordinate) };
    const auto& neighbors{ node.GetNeighbors() };
    check(neighbors.size() == 2);
    PipeGridCoordinate start{ -1, -1 };
    PipeGridCoordinate end{ -1, -1 };
    for (const auto* neighbor : neighbors)
    {
        if (start == PipeGridCoordinate{ -1, -1 })
        {
            start = neighbor->first;
        }
        else if (end == PipeGridCoordinate{ -1, -1 })
        {
            end = neighbor->first;
        }
    }

    std::vector<PipeGridCoordinate> path{ };
    check(hotSprings.pipeGrid.TryFindPath(path, start, end));
    std::cout << (path.size() + 1) / 2 << std::endl;
}

void Puzzle10::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle10Helpers;

    auto hotSprings{ CreateHotSprings(input) };
}
