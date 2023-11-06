#pragma once

#include "pch.h"

#include "Grid2d.h"

namespace Core::Math
{
	template<typename T>
	class NavGrid2d
	{
	public:
		NavGrid2d(const Grid2d<T>& inGrid)
			: grid{ inGrid.GetWidth(), inGrid.GetHeight() }
		{
			InitializeNavGrid(inGrid);
		}

	private:
		struct Node
		{
			T value;
			std::pair<size_t, size_t> parent;
			float totalCost{ 0.f };		// f
			float pathCost{ 0.f };		// g
			float heuristicCost{ 0.f };	// h
		};

		void InitializeNavGrid(const Grid2d<T>& inGrid);

	private:
		Grid2d<Node> grid;
	};

	template<typename T>
	void NavGrid2d<T>::InitializeNavGrid(const Grid2d<T>& inGrid)
	{
		for (size_t y{ 0 }; y < inGrid.GetHeight(); ++y)
		{
			for (size_t x{ 0 }; x < inGrid.GetWidth(); ++x)
			{
				grid.at(x, y).value = inGrid.at(x, y);
			}
		}
	}
}
