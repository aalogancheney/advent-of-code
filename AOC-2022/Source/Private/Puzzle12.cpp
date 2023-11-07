#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle12Helpers
{
	using GroundHeight = char;

	class TopologicalGrid : public Core::Math::Grid2d<GroundHeight>
	{
	public:
		TopologicalGrid(size_t inWidth, size_t inHeight)
			: Grid2d{ inWidth, inHeight }
		{

		}

		size_t startX{ 0 };
		size_t startY{ 0 };

		size_t endX{ 0 };
		size_t endY{ 0 };
	};

	using TopologicalNavGrid = Core::Math::NavGrid2d<GroundHeight>;

	constexpr GroundHeight start{ 'S' };
	constexpr GroundHeight startEval{ 'a' };
	constexpr GroundHeight end{ 'E' };
	constexpr GroundHeight endEval{ 'z' };

	TopologicalNavGrid::AdjacencyCriteria hillClimbingAdjacencyCriteria = [](const GroundHeight& nodeHeight, const GroundHeight& neighborHeight)
	{
		if (nodeHeight == start)
		{
			return neighborHeight == startEval;
		}
		if (nodeHeight == end)
		{
			return neighborHeight == endEval;
		}
		return neighborHeight - 1 <= nodeHeight;
	};

	TopologicalGrid CreateGrid(const std::vector<std::string>& input)
	{
		TopologicalGrid grid{ input[0].length(), input.size() };
		for (size_t y{ 0 }; y < input.size(); ++y)
		{
			const std::string& line{ input[y] };
			for (size_t x{ 0 }; x < line.length(); ++x)
			{
				grid.at(x, y) = line[x];

				if (line[x] == start)
				{
					grid.startX = x;
					grid.startY = y;
					grid.at(x, y) = startEval;
				}
				else if (line[x] == end)
				{
					grid.endX = x;
					grid.endY = y;
					grid.at(x, y) = endEval;
				}
			}
		}
		return grid;
	}
}

void Puzzle12::SolveA(const std::vector<std::string>& input) const
{
	using namespace Puzzle12Helpers;

	TopologicalGrid grid{ CreateGrid(input) };
	TopologicalNavGrid navGrid{ grid, hillClimbingAdjacencyCriteria };

	size_t pathLength{ navGrid.GetPathLength(grid.startX, grid.startY, grid.endX, grid.endY) };
	std::cout << "Path length = " << pathLength << std::endl;
}

void Puzzle12::SolveB(const std::vector<std::string>& input) const
{
	using namespace Puzzle12Helpers;

	TopologicalGrid grid{ CreateGrid(input) };
	TopologicalNavGrid navGrid{ grid, hillClimbingAdjacencyCriteria };

	// Definitely not optimal--should instead be able to search from the end point and go outward
	// to find the nearest 'a'. Runtime as-is is ~5s.
	std::vector<size_t> pathLengths{ };
	for (size_t y{ 0 }; y < grid.GetHeight(); ++y)
	{
		for (size_t x{ 0 }; x < grid.GetWidth(); ++x)
		{
			if (grid.at(x, y) == start || grid.at(x, y) == startEval)
			{
				size_t pathLength{ navGrid.GetPathLength(x, y, grid.endX, grid.endY) };
				if (pathLength != 0)
				{
					pathLengths.emplace_back(pathLength);
				}
			}
		}
	}

	size_t minimumPathLength{ pathLengths[0] };
	for (size_t pathLength : pathLengths)
	{
		if (pathLength < minimumPathLength)
		{
			minimumPathLength = pathLength;
		}
	}

	std::cout << "Path length = " << minimumPathLength << std::endl;
}
