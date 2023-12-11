#pragma once

#include "pch.h"

#include "Grid2d.h"
#include "Vector2.h"
#include "Concepts.h"

namespace Core::Math
{
    // A simple 2D grid that can calculate paths. Two pathfinding options are available:
    // 1. Find a path from one position to another known position. This uses A* to find the best available path.
    // 2. Find a path from one position to another unknown position (searching for a particular value). This is
    //    useful if you don't know exactly where your target position is, or if there are multiple candidates.
    //    This uses A* with a heuristic value of 0, which degenerates to Dijkstra's, so an optimal path will be found.
    template<typename TElement, SignedIntegral TSize>
    class NavGrid2d
    {
    public:
        struct Node
        {
            TElement value{ };

            const std::unordered_set<typename Grid2d<Node, TSize>::Grid2dElement*> GetNeighbors() const { return adjacencyList; }

        private:
            // Allow NavGrid2d access to private pathfinding variables.
            friend class NavGrid2d;

            // Pathfinding member variables.
            Grid2d<Node, TSize>::Grid2dElement* parent{ nullptr };
            std::unordered_set<typename Grid2d<Node, TSize>::Grid2dElement*> adjacencyList{ };
            TSize pathCost{ 0 };      // g
            TSize heuristicCost{ 0 }; // h
            TSize GetTotalCost() const { return pathCost + heuristicCost; }
        };
        
        using NavGrid2dElement = Grid2d<Node, TSize>::Grid2dElement;
        using NavGrid2dCoordinate = Grid2d<Node, TSize>::Grid2dCoordinate;

        using AssignGridValues = std::function<void(NavGrid2dElement&)>;

        // Additional criteria can be applied when building the adjacency list for each node. The first argument is the current
        // node; the second argument is the neighboring node.
        using AdjacencyCriteria = std::function<bool(const NavGrid2dElement&, const NavGrid2dElement&)>;

        // Default adjacency criteria that does no filtering. This is probably undesired for most use-cases, since a complete
        // adjacency set will degenerate into moving diagonally/horizontally until the target is reached.
        static AdjacencyCriteria allNeighbors;

        NavGrid2d(TSize inWidth, TSize inHeight, AssignGridValues assignGridValues, AdjacencyCriteria adjacencyCriteria)
            : grid{ inWidth, inHeight }
        {
            for (auto& element : *this)
            {
                assignGridValues(element);
            }
            BuildAdjacencyList(adjacencyCriteria);
        }

        auto begin() noexcept { return grid.begin(); }
        auto end() noexcept { return grid.end(); }
        auto begin() const noexcept { return grid.begin(); }
        auto end() const noexcept { return grid.end(); }
        auto cbegin() const noexcept { return grid.cbegin(); }
        auto cend() const noexcept { return grid.cend(); }
        auto rbegin() noexcept { return grid.rbegin(); }
        auto rend() noexcept { return grid.rend(); }
        auto rbegin() const noexcept { return grid.rbegin(); }
        auto rend() const noexcept { return grid.rend(); }
        auto crbegin() const noexcept { return grid.crbegin(); }
        auto crend() const noexcept { return grid.crend(); }

        constexpr const NavGrid2dElement& at(NavGrid2dCoordinate coordinate) const
        {
            return grid.at(coordinate);
        }

        constexpr NavGrid2dElement& at(NavGrid2dCoordinate coordinate)
        {
            return grid.at(coordinate);
        }

        // Tries to find a path connecting "from" and "to" positions on the grid. Returns true if a path was found, and
        // the outPath vector will be filled with the steps (including the start and end points). Returns false if no path
        // could be found.
        bool TryFindPath(std::vector<NavGrid2dCoordinate>& outPath, const NavGrid2dCoordinate& from, const NavGrid2dCoordinate& to);
        
        // Tries to find a path connecting "from" and any position in the grid with the provided value. This will always
        // find the shortest path. Returns true if a path was found, and the outPath vector will be filled with the steps
        // (including the start and end points). Returns false if no path could be found.
        bool TryFindPath(std::vector<NavGrid2dCoordinate>& outPath, const NavGrid2dCoordinate& from, const TElement& any);

    private:
        Grid2d<Node, TSize> grid;

        void BuildAdjacencyList(AdjacencyCriteria adjacencyCriteria);
        void ResetSearch();
        void BuildHeuristicCosts(const NavGrid2dCoordinate& to);

        // Implements A* pathfinding. Different acceptance criteria can be provided to search for a particular candidate node.
        using AcceptanceCriteria = std::function<bool(const NavGrid2dElement*)>;
        bool TryFindPathInternal(std::vector<NavGrid2dCoordinate>& outPath, const NavGrid2dCoordinate& from, AcceptanceCriteria acceptanceCriteria);
    };

    template<typename TElement, SignedIntegral TSize>
    NavGrid2d<TElement, TSize>::AdjacencyCriteria NavGrid2d<TElement, TSize>::allNeighbors{ [](const TElement&, const TElement&) { return true; } };

    template<typename TElement, SignedIntegral TSize>
    bool NavGrid2d<TElement, TSize>::TryFindPath(std::vector<NavGrid2dCoordinate>& outPath, const NavGrid2dCoordinate& from, const NavGrid2dCoordinate& to)
    {
        ResetSearch();
        BuildHeuristicCosts(to); // With a given target, heuristics can be built to decrease the number of visited nodes.
        return TryFindPathInternal(outPath, from, [&to](const NavGrid2dElement* candidate) {
            const auto& [coordinate, node] { *candidate };
            return coordinate == to;
        });
    }

    template<typename TElement, SignedIntegral TSize>
    bool NavGrid2d<TElement, TSize>::TryFindPath(std::vector<NavGrid2dCoordinate>& outPath, const NavGrid2dCoordinate& from, const TElement& any)
    {
        ResetSearch();
        return TryFindPathInternal(outPath, from, [&any](const NavGrid2dElement* candidate) {
            const auto& [coordinate, node] { *candidate };
            return node.value == any;
        });
    }

    template<typename TElement, SignedIntegral TSize>
    bool NavGrid2d<TElement, TSize>::TryFindPathInternal(std::vector<NavGrid2dCoordinate>& outPath, const NavGrid2dCoordinate& from, AcceptanceCriteria acceptanceCriteria)
    {
        outPath.clear();

        NavGrid2dElement* currentElement{ &grid.at(from) };

        std::unordered_set<NavGrid2dElement*> closedSet{ };
        closedSet.emplace(currentElement);

        // The openSet should ideally be a priority queue to automatically sort candidate nodes
        // based on their total cost. However, the std::priority_queue implementation does not
        // support a .contains() method. The candidates in the openSet should be small enough
        // that this won't have a major impact.
        std::unordered_set<NavGrid2dElement*> openSet{ };

        // Continue searching until the acceptance criteria is met.
        while (!acceptanceCriteria(currentElement))
        {
            auto& [currentCoordinate, currentNode] { *currentElement };

            // Check each neighbor.
            for (NavGrid2dElement* neighbor : currentNode.adjacencyList)
            {
                // If it has been fully explored (i.e. in the closedSet), ignore and move on.
                if (closedSet.contains(neighbor)) { continue; }

                auto& [neighborCoordinate, neighborNode] { *neighbor };

                // If it has been visited, but not fully explored (i.e. in the openSet), check if
                // parental adoption should take place. This occurs when the current path cost is
                // cheaper than the previously-calculated version (i.e. we found a better path from
                // the starting position to the candidate node).
                if (openSet.contains(neighbor))
                {
                    auto newPathCost{ currentNode.pathCost + 1 };
                    if (newPathCost < neighborNode.pathCost)
                    {
                        neighborNode.parent = currentElement;
                        neighborNode.pathCost = newPathCost;
                    }
                }
                else
                {
                    neighborNode.parent = currentElement;
                    neighborNode.pathCost = currentNode.pathCost + 1;
                    openSet.emplace(neighbor);
                }
            }

            // If there are no nodes left to explore, we've either found the path or no path exists.
            if (openSet.empty()) { break; }

            // Find the next-best candidate based on the total cost of reaching that node.
            auto iter{ openSet.begin() };
            NavGrid2dElement* nextElement{ *iter };
            while (iter != openSet.end())
            {
                const auto& [coordinate, node] { **iter };
                if (node.GetTotalCost() < nextElement->second.GetTotalCost())
                {
                    nextElement = *iter;
                }
                ++iter;
            }

            // Update pointers and visited nodes.
            currentElement = nextElement;
            openSet.erase(nextElement);
            closedSet.emplace(nextElement);
        }

        // If the acceptance criteria is met and the node has a parent (meaning the path can be reconstructed),
        // generate that path.
        if (acceptanceCriteria(currentElement) && currentElement->second.parent != nullptr)
        {
            while (currentElement != nullptr)
            {
                outPath.emplace_back(currentElement->first);
                currentElement = currentElement->second.parent;
            }
        }
        return outPath.size() > 0;
    }

    template<typename TElement, SignedIntegral TSize>
    void NavGrid2d<TElement, TSize>::BuildAdjacencyList(AdjacencyCriteria adjacencyCriteria)
    {
        for (auto& element : *this)
        {
            auto& [coordinate, node] { element };
            std::vector<NavGrid2dElement*> cartesianNeighbors;
            for (const auto& cartesianNeighboringDirection : NavGrid2dCoordinate::cardinalNeighboringDirections)
            {
                if (grid.IsInBounds(coordinate + cartesianNeighboringDirection))
                {
                    cartesianNeighbors.emplace_back(&grid.at(coordinate + cartesianNeighboringDirection));
                }
            }
            for (NavGrid2dElement* cartesianNeighbor : cartesianNeighbors)
            {
                if (adjacencyCriteria(element, *cartesianNeighbor))
                {
                    node.adjacencyList.emplace(cartesianNeighbor);
                }
            }
        }
    }

    template<typename TElement, SignedIntegral TSize>
    void NavGrid2d<TElement, TSize>::BuildHeuristicCosts(const NavGrid2dCoordinate& to)
    {
        for (auto& [coordinate, node] : *this)
        {
            node.heuristicCost = std::abs(coordinate.x - to.x) + std::abs(coordinate.y - to.y) - 1;
        }
    }

    template<typename TElement, SignedIntegral TSize>
    void NavGrid2d<TElement, TSize>::ResetSearch()
    {
        for (auto& [coordinate, node] : *this)
        {
            node.pathCost = node.heuristicCost = 0;
            node.parent = nullptr;
        }
    }
}
