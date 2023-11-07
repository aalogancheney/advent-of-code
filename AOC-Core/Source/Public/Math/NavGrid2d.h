#pragma once

#include "pch.h"

#include "Grid2d.h"

namespace Core::Math
{
	// TODO: This currently implements Dijkstra's Algorithm since the heuristic is ignored. Expose a heuristic for users to configure when searching.
	template<typename T>
	class NavGrid2d
	{
	public:
		using AdjacencyCriteria = std::function<bool(const T&, const T&)>;

		// Standard manhattan adjacency criteria that will allow neighbors in the cardinal directions to be considered.
		static AdjacencyCriteria manhattanAdjacencyCriteria;

		NavGrid2d(const Grid2d<T>& inGrid, AdjacencyCriteria adjacencyCriteria)
			: grid{ inGrid.GetWidth(), inGrid.GetHeight() }
		{
			InitializeNavGrid(inGrid);
			BuildAdjacencyList(adjacencyCriteria);
		}

		// TODO: This finds the length of the path. Generalize this functionality to return a path for users to interact with.
		size_t GetPathLength(size_t startX, size_t startY, size_t endX, size_t endY);

	private:
		struct Node
		{
			T value{ };
			size_t x{ };
			size_t y{ };
			Node* parent{ nullptr };
			std::unordered_set<Node*> adjacencyList{ };
			float totalCost{ 0.f };		// f
			float pathCost{ 0.f };		// g
			float heuristicCost{ 0.f };	// h
		};

		void InitializeNavGrid(const Grid2d<T>& inGrid);
		void BuildAdjacencyList(AdjacencyCriteria adjacencyCriteria);

		void ResetSearch();

	private:
		Grid2d<Node> grid;
	};

	template<typename T>
	NavGrid2d<T>::AdjacencyCriteria NavGrid2d<T>::manhattanAdjacencyCriteria{ [](const T&, const T&) { return true; } };

	template<typename T>
	size_t NavGrid2d<T>::GetPathLength(size_t startX, size_t startY, size_t endX, size_t endY)
	{
		ResetSearch();

		Node* currentNode{ &grid.at(startX, startY) };
		Node* targetNode{ &grid.at(endX, endY) };
		// TODO: Given a current node and a target node, calculate an admissable heuristic. This could likely be the
		// manhattan distance.

		currentNode->pathCost = 0.f;
		currentNode->parent = nullptr;

		std::unordered_set<Node*> closedSet{ };
		closedSet.emplace(currentNode);

		std::unordered_set<Node*> openSet{ };

		while (currentNode != targetNode)
		{
			for (Node* candidate : currentNode->adjacencyList)
			{
				if (closedSet.contains(candidate)) { continue; }

				if (openSet.contains(candidate))
				{
					float newPathCost{ currentNode->pathCost + 1 };
					if (newPathCost < candidate->pathCost)
					{
						candidate->parent = currentNode;
						candidate->pathCost = newPathCost;
						candidate->totalCost = candidate->pathCost + candidate->heuristicCost;
					}
				}
				else
				{
					candidate->parent = currentNode;
					candidate->pathCost = currentNode->pathCost + 1;
					candidate->totalCost = candidate->pathCost + candidate->heuristicCost;
					openSet.emplace(candidate);
				}
			}

			if (openSet.empty()) { break; }

			auto iter{ openSet.begin() };
			Node* nextNode{ *iter };
			while (iter != openSet.end())
			{
				if ((*iter)->totalCost < nextNode->totalCost)
				{
					nextNode = *iter;
				}
				++iter;
			}

			currentNode = nextNode;
			openSet.erase(currentNode);
			closedSet.emplace(currentNode);
		}

		// This is the part that should be generalized. The path exists, but is baked into the nodes in the nav grid.
		size_t pathLength{ 0 };
		if (targetNode->parent != nullptr)
		{
			Node* traversal{ targetNode };
			while (traversal->parent != nullptr)
			{
				++pathLength;
				traversal = traversal->parent;
			}
		}
		return pathLength;
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
				node.x = x;
				node.y = y;
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
				if (node.x > 0) { neighbors.emplace_back(&grid.at(node.x - 1, node.y)); }
				if (node.x < grid.GetWidth() - 1) { neighbors.emplace_back(&grid.at(node.x + 1, node.y)); }
				if (node.y > 0) { neighbors.emplace_back(&grid.at(node.x, node.y - 1)); }
				if (node.y < grid.GetHeight() - 1) { neighbors.emplace_back(&grid.at(node.x, node.y + 1)); }

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
	void NavGrid2d<T>::ResetSearch()
	{
		// TODO: Build iterable version of Grid2d.
		for (size_t y{ 0 }; y < grid.GetHeight(); ++y)
		{
			for (size_t x{ 0 }; x < grid.GetWidth(); ++x)
			{
				Node& node{ grid.at(x, y) };
				node.pathCost = node.heuristicCost = node.totalCost = 0.f;
				node.parent = nullptr;
			}
		}
	}
}
