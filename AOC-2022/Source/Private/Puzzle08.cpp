#include "pch.h"

#include "Puzzles.h"
#include "Core.h"

using TreeHeight = int32;
using TreeGrid = Core::Math::Grid2d<TreeHeight>;

namespace Puzzle08Helpers
{
	TreeGrid CreateGrid(const std::vector<std::string>& input)
	{
		TreeGrid treeGrid{ input.size(), input.size() };
		for (auto rowIndex{ 0 }; rowIndex < input.size(); ++rowIndex)
		{
			const auto& line{ input[rowIndex] };
			for (auto colIndex{ 0 }; colIndex < line.size(); ++colIndex)
			{
				treeGrid.at(rowIndex, colIndex) = static_cast<TreeHeight>(line[colIndex] - '0');
			}
		}
		return treeGrid;
	}

	bool CheckColumnVisibility(const TreeGrid& treeGrid, const size_t x, const size_t y)
	{
		const auto treeHeight{ treeGrid.at(x, y) };
		auto visibleAbove{ true };
		auto visibleBelow{ true };
		for (auto row{ 0 }; row < treeGrid.GetHeight(); ++row)
		{
			if (row < y)
			{
				visibleAbove &= treeGrid.at(x, row) < treeHeight;
			}
			else if (row > y)
			{
				visibleBelow &= treeGrid.at(x, row) < treeHeight;
			}
		}
		return visibleAbove || visibleBelow;
	}

	bool CheckRowVisibility(const TreeGrid& treeGrid, const size_t x, const size_t y)
	{
		const auto treeHeight{ treeGrid.at(x, y) };
		auto visibleLeft{ true };
		auto visibleRight{ true };
		for (auto col{ 0 }; col < treeGrid.GetWidth(); ++col)
		{
			if (col < x)
			{
				visibleLeft &= treeGrid.at(col, y) < treeHeight;
			}
			else if (col > x)
			{
				visibleRight &= treeGrid.at(col, y) < treeHeight;
			}
		}
		return visibleLeft || visibleRight;
	}

	bool CheckVisibility(const TreeGrid& treeGrid, const size_t x, const size_t y)
	{
		return x == 0 || x == treeGrid.GetWidth() - 1 || y == 0 || y == treeGrid.GetHeight() - 1 ||
			CheckColumnVisibility(treeGrid, x, y) || CheckRowVisibility(treeGrid, x, y);
	}

	TreeGrid GetVisibilityScoreGrid(const TreeGrid& treeGrid)
	{
		TreeGrid visibilityScores{ treeGrid.GetWidth(), treeGrid.GetHeight() };
		for (auto row{ 1 }; row < treeGrid.GetWidth() - 1; ++row)
		{
			for (auto col{ 1 }; col < treeGrid.GetHeight() - 1; ++col)
			{
				const auto treeHeight{ treeGrid.at(row, col) };

				auto visibleAbove{ 1 };
				for (auto index{ row - 1 }; index > 0 && treeGrid.at(index, col) < treeHeight; --index, ++visibleAbove);

				auto visibleBelow{ 1 };
				for (auto index{ row + 1 }; index < treeGrid.GetHeight() - 1 && treeGrid.at(index, col) < treeHeight; ++index, ++visibleBelow);

				auto visibleLeft{ 1 };
				for (auto index{ col - 1 }; index > 0 && treeGrid.at(row, index) < treeHeight; --index, ++visibleLeft);

				auto visibleRight{ 1 };
				for (auto index{ col + 1 }; index < treeGrid.GetWidth() - 1 && treeGrid.at(row, index) < treeHeight; ++index, ++visibleRight);

				visibilityScores.at(row, col) = visibleAbove * visibleBelow * visibleLeft * visibleRight;
			}
		}
		return visibilityScores;
	}
}

void Puzzle08::SolveA(const std::vector<std::string>& input) const
{
	using namespace Puzzle08Helpers;

	TreeGrid treeGrid{ CreateGrid(input) };

	auto visibleTrees{ 0 };
	for (auto row{ 0 }; row < treeGrid.GetWidth(); ++row)
	{
		for (auto col{ 0 }; col < treeGrid.GetHeight(); ++col)
		{
			visibleTrees += CheckVisibility(treeGrid, row, col);
		}
	}

	std::cout << visibleTrees << std::endl;
}

void Puzzle08::SolveB(const std::vector<std::string>& input) const
{
	using namespace Puzzle08Helpers;

	TreeGrid treeGrid{ CreateGrid(input) };
	TreeGrid visibilityScoreGrid{ GetVisibilityScoreGrid(treeGrid) };
	auto max{ 0 };
	for (auto row{ 0 }; row < visibilityScoreGrid.GetWidth(); ++row)
	{
		for (auto col{ 0 }; col < visibilityScoreGrid.GetHeight(); ++col)
		{
			max = std::max(max, visibilityScoreGrid.at(row, col));
		}
	}
	std::cout << max << std::endl;
}
