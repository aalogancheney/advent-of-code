#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

using TreeGrid = Core::Math::Grid2d<int32>;

namespace Puzzle08Helpers
{
	TreeGrid CreateGrid(const std::vector<std::string>& input)
	{
		TreeGrid treeGrid{ input.size(), input.size() };
		for (int32 rowIndex{ 0 }; rowIndex < input.size(); ++rowIndex)
		{
			const auto& line{ input[rowIndex] };
			for (int32 colIndex{ 0 }; colIndex < line.size(); ++colIndex)
			{
				treeGrid.at(rowIndex, colIndex) = static_cast<uint8>(line[colIndex] - '0');
			}
		}
		return treeGrid;
	}
}

void Puzzle08::SolveA(const std::vector<std::string>& input) const
{
	using namespace Puzzle08Helpers;

	TreeGrid treeGrid{ CreateGrid(input) };
	treeGrid.Print();
}

void Puzzle08::SolveB(const std::vector<std::string>& input) const
{
	using namespace Puzzle08Helpers;


}
