#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

using GroundHeight = char;
using TopologicalGrid = Core::Math::Grid2d<GroundHeight>;

namespace Puzzle12Helpers
{
	TopologicalGrid CreateGrid(const std::vector<std::string>& input)
	{
		TopologicalGrid grid{ input[0].length(), input.size() };
		for (size_t y{ 0 }; y < input.size(); ++y)
		{
			const std::string& line{ input[y] };
			for (size_t x{ 0 }; x < line.length(); ++x)
			{
				grid.at(x, y) = line[x];
			}
		}
		return grid;
	}
}

void Puzzle12::SolveA(const std::vector<std::string>& input) const
{
	using namespace Puzzle12Helpers;

	TopologicalGrid grid{ CreateGrid(input) };
	grid.Print();
}

void Puzzle12::SolveB(const std::vector<std::string>& input) const
{
	using namespace Puzzle12Helpers;

	TopologicalGrid grid{ CreateGrid(input) };
}
