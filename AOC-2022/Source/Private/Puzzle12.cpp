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

		Grid2dCoordinate start{ 0, 0 };
		Grid2dCoordinate end{ 0, 0 };
	};

	using TopologicalNavGrid = Core::Math::NavGrid2d<GroundHeight>;

	constexpr GroundHeight start{ 'S' };
	constexpr GroundHeight startEval{ 'a' };
	constexpr GroundHeight end{ 'E' };
	constexpr GroundHeight endEval{ 'z' };

	TopologicalGrid CreateGrid(const std::vector<std::string>& input)
	{
		TopologicalGrid grid{ input[0].length(), input.size() };
		for (size_t y{ 0 }; y < input.size(); ++y)
		{
			const std::string& line{ input[y] };
			for (size_t x{ 0 }; x < line.length(); ++x)
			{
				grid.at(x, y) = line[x];

				// Cache the start/end points, but replace them with their corresponding
				// values for pathfinding.
				if (line[x] == start)
				{
					grid.start.x = x;
					grid.start.y = y;
					grid.at(x, y) = startEval;
				}
				else if (line[x] == end)
				{
					grid.end.x = x;
					grid.end.y = y;
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

	TopologicalNavGrid::AdjacencyCriteria hillClimbingAdjacencyCriteria = [](const GroundHeight& nodeHeight, const GroundHeight& neighborHeight)
	{
		// When climbing, neighbors can be at most 1 higher than our current position, or they can be as low as we want.
		return neighborHeight - 1 <= nodeHeight;
	};
	TopologicalNavGrid navGrid{ grid, hillClimbingAdjacencyCriteria };

	std::vector<Grid2dCoordinate> path;
	check(navGrid.TryFindPath(path, grid.start, grid.end));

	// Subtract 1 since the path contains the start and end points.
	std::cout << "Path length = " << path.size() - 1 << std::endl;
}

void Puzzle12::SolveB(const std::vector<std::string>& input) const
{
	using namespace Puzzle12Helpers;

	TopologicalGrid grid{ CreateGrid(input) };

	TopologicalNavGrid::AdjacencyCriteria hillDescendingAdjacencyCriteria = [](const GroundHeight& nodeHeight, const GroundHeight& neighborHeight)
	{
		// When descending, neighbors can be at most 1 lower than our current position. Since we're searching in reverse, this condition
		// ensures that we only get to our current position from a valid neighbor.
		return neighborHeight + 1 >= nodeHeight;
	};
	TopologicalNavGrid navGrid{ grid, hillDescendingAdjacencyCriteria };

	std::vector<Grid2dCoordinate> path;
	check(navGrid.TryFindPath(path, grid.end, 'a'));

	// Subtract 1 since the path contains the start and end points.
	std::cout << "Path length = " << path.size() - 1 << std::endl;
}
