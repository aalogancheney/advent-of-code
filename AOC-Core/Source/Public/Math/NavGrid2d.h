#pragma once

#include "pch.h"

#include "Grid2d.h"
#include "Vector2d.h"

using Grid2dCoordinate = Core::Math::Vector2D<size_t>;

namespace Core::Math
{
    // A simple 2D grid that can calculate paths. Two pathfinding options are available:
    // 1. Find a path from one position to another known position. This uses A* to find the best available path.
    // 2. Find a path from one position to another unknown position (searching for a particular value). This is
    //    useful if you don't know exactly where your target position is, or if there are multiple candidates.
    //    This uses A* with a heuristic value of 0, which degenerates to Dijkstra's, so an optimal path will be found.
    template<typename T>
    class NavGrid2d
    {
    public:
        // Additional criteria can be applied when building the adjacency list for each node. The first argument is the current
        // node's value; the second argument is the neighboring node's value.
        using AdjacencyCriteria = std::function<bool(const T&, const T&)>;

        // Default adjacency criteria that does no filtering. This is probably undesired for most use-cases, since a complete
        // adjacency set will degenerate into moving diagonally/horizontally until the target is reached, since there are no barriers.
        static AdjacencyCriteria allNeighbors;

        NavGrid2d(const Grid2d<T>& inGrid, AdjacencyCriteria adjacencyCriteria)
            : grid{ inGrid.GetWidth(), inGrid.GetHeight() }
        {
            InitializeNavGrid(inGrid);
            BuildAdjacencyList(adjacencyCriteria);
        }

        // Tries to find a path connecting "from" and "to" positions on the grid. Returns true if a path was found, and
        // the outPath vector will be filled with the steps (including the start and end points). Returns false if no path
        // could be found.
        bool TryFindPath(std::vector<Grid2dCoordinate>& outPath, const Grid2dCoordinate& from, const Grid2dCoordinate& to);
        
        // Tries to find a path connecting "from" and any position in the grid with the provided value. This will always
        // find the shortest path. Returns true if a path was found, and the outPath vector will be filled with the steps
        // (including the start and end points). Returns false if no path could be found.
        bool TryFindPath(std::vector<Grid2dCoordinate>& outPath, const Grid2dCoordinate& from, const T& any);

    private:
        // Helper struct to gather all information needed to search.
        struct Node
        {
            T value{ };
            Grid2dCoordinate coordinate{ };
            Node* parent{ nullptr };
            std::unordered_set<Node*> adjacencyList{ };
            size_t pathCost{ 0 };        // g
            size_t heuristicCost{ 0 };    // h

            size_t GetTotalCost() const { return pathCost + heuristicCost; }
        };

        void InitializeNavGrid(const Grid2d<T>& inGrid);
        void BuildAdjacencyList(AdjacencyCriteria adjacencyCriteria);
        void ResetSearch();
        void BuildHeuristicCosts(const Grid2dCoordinate& to);

        // Implements A* pathfinding. Different acceptance criteria can be provided to search for a particular candidate node.
        using AcceptanceCriteria = std::function<bool(Node*)>;
        bool TryFindPathInternal(std::vector<Grid2dCoordinate>& outPath, const Grid2dCoordinate& from, AcceptanceCriteria acceptanceCriteria);

    private:
        Grid2d<Node> grid;
    };

    template<typename T>
    NavGrid2d<T>::AdjacencyCriteria NavGrid2d<T>::allNeighbors{ [](const T&, const T&) { return true; } };

    template<typename T>
    bool NavGrid2d<T>::TryFindPath(std::vector<Grid2dCoordinate>& outPath, const Grid2dCoordinate& from, const Grid2dCoordinate& to)
    {
        ResetSearch();
        BuildHeuristicCosts(to); // With a given target, heuristics can be built to decrease the number of visited nodes.
        return TryFindPathInternal(outPath, from, [&to](Node* candidate)
            {
                return candidate->coordinate == to;
            });
    }

    template<typename T>
    bool NavGrid2d<T>::TryFindPath(std::vector<Grid2dCoordinate>& outPath, const Grid2dCoordinate& from, const T& any)
    {
        ResetSearch();
        return TryFindPathInternal(outPath, from, [&any](Node* candidate)
            {
                return candidate->value == any;
            });
    }

    template<typename T>
    bool NavGrid2d<T>::TryFindPathInternal(std::vector<Grid2dCoordinate>& outPath, const Grid2dCoordinate& from, AcceptanceCriteria acceptanceCriteria)
    {
        outPath.clear();

        Node* currentNode{ &grid.at(from.x, from.y) };

        std::unordered_set<Node*> closedSet{ };
        closedSet.emplace(currentNode);

        // The openSet should ideally be a priority queue to automatically sort candidate nodes
        // based on their total cost. However, the std::priority_queue implementation does not
        // support a .contains() method. The candidates in the openSet should be small enough
        // that this won't have a major impact.
        std::unordered_set<Node*> openSet{ };

        // Continue searching until the acceptance criteria is met.
        while (!acceptanceCriteria(currentNode))
        {
            // Check each neighbor.
            for (Node* candidate : currentNode->adjacencyList)
            {
                // If it has been fully explored (i.e. in the closedSet), ignore and move on.
                if (closedSet.contains(candidate)) { continue; }

                // If it has been visited, but not fully explored (i.e. in the openSet), check if
                // parental adoption should take place. This occurs when the current path cost is
                // cheaper than the previously-calculated version (i.e. we found a better path from
                // the starting position to the candidate node).
                if (openSet.contains(candidate))
                {
                    size_t newPathCost{ currentNode->pathCost + 1 };
                    if (newPathCost < candidate->pathCost)
                    {
                        candidate->parent = currentNode;
                        candidate->pathCost = newPathCost;
                    }
                }
                else
                {
                    candidate->parent = currentNode;
                    candidate->pathCost = currentNode->pathCost + 1;
                    openSet.emplace(candidate);
                }
            }

            // If there are no nodes left to explore, we've either found the path or no path exists.
            if (openSet.empty()) { break; }

            // Find the next-best candidate based on the total cost of reaching that node.
            auto iter{ openSet.begin() };
            Node* nextNode{ *iter };
            while (iter != openSet.end())
            {
                if ((*iter)->GetTotalCost() < nextNode->GetTotalCost())
                {
                    nextNode = *iter;
                }
                ++iter;
            }

            // Update pointers and visited nodes.
            currentNode = nextNode;
            openSet.erase(currentNode);
            closedSet.emplace(currentNode);
        }

        // If the acceptance criteria is met and the node has a parent (meaning the path can be reconstructed),
        // generate that path.
        if (acceptanceCriteria(currentNode) && currentNode->parent != nullptr)
        {
            while (currentNode != nullptr)
            {
                outPath.emplace_back(currentNode->coordinate);
                currentNode = currentNode->parent;
            }
        }
        return outPath.size() > 0;
    }

    template<typename T>
    void NavGrid2d<T>::InitializeNavGrid(const Grid2d<T>& inGrid)
    {
        for (size_t y{ 0 }; y < inGrid.GetHeight(); ++y)
        {
            for (size_t x{ 0 }; x < inGrid.GetWidth(); ++x)
            {
                Node& node{ grid.at(x, y) };
                node.value = inGrid.at(x, y);
                node.coordinate.x = x;
                node.coordinate.y = y;
            }
        }
    }

    template<typename T>
    void NavGrid2d<T>::BuildAdjacencyList(AdjacencyCriteria adjacencyCriteria)
    {
        // TODO: Build iterable version of Grid2d.
        for (size_t y{ 0 }; y < grid.GetHeight(); ++y)
        {
            for (size_t x{ 0 }; x < grid.GetWidth(); ++x)
            {
                Node& node{ grid.at(x, y) };
                
                // Consider neighbors in the cardinal directions.
                // TODO: Offer option to consider diagonal neighbors?
                std::vector<Node*> neighbors{ };
                if (node.coordinate.x > 0)                        { neighbors.emplace_back(&grid.at(node.coordinate.x - 1, node.coordinate.y)); }
                if (node.coordinate.x < grid.GetWidth() - 1)    { neighbors.emplace_back(&grid.at(node.coordinate.x + 1, node.coordinate.y)); }
                if (node.coordinate.y > 0)                        { neighbors.emplace_back(&grid.at(node.coordinate.x, node.coordinate.y - 1)); }
                if (node.coordinate.y < grid.GetHeight() - 1)    { neighbors.emplace_back(&grid.at(node.coordinate.x, node.coordinate.y + 1)); }

                // For each neighbor, apply the AdjacencyCriteria to see if we ACTUALLY want it to be considered as a neighbor.
                // This is a more strict filter that can be used to determine whether or not a neighbor is worth visiting according
                // to user-defined criteria.
                for (Node* neighbor : neighbors)
                {
                    if (adjacencyCriteria(node.value, neighbor->value))
                    {
                        node.adjacencyList.emplace(neighbor);
                    }
                }
            }
        }
    }

    template<typename T>
    void NavGrid2d<T>::BuildHeuristicCosts(const Grid2dCoordinate& to)
    {
        // TODO: Build iterable version of Grid2d.
        for (size_t y{ 0 }; y < grid.GetHeight(); ++y)
        {
            for (size_t x{ 0 }; x < grid.GetWidth(); ++x)
            {
                Node& node{ grid.at(x, y) };

                // Heuristic cost is the manhattan distance. Keeps the values as integrals.
                node.heuristicCost = (x > to.x ? x - to.x : to.x - x) + (y > to.y ? y - to.y : to.y - y) - 1;
            }
        }
    }

    template<typename T>
    void NavGrid2d<T>::ResetSearch()
    {
        // TODO: Build iterable version of Grid2d.
        for (size_t y{ 0 }; y < grid.GetHeight(); ++y)
        {
            for (size_t x{ 0 }; x < grid.GetWidth(); ++x)
            {
                Node& node{ grid.at(x, y) };
                node.pathCost = node.heuristicCost = 0;
                node.parent = nullptr;
            }
        }
    }
}
