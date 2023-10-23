#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle09Helpers
{
	using namespace Core;
	using namespace Core::Math;

	void UpdateHeadPosition(const char direction, IVector2D& headPosition)
	{
		switch (direction)
		{
		case 'U':
			headPosition += IVector2D::up;
			break;
		case 'D':
			headPosition += IVector2D::down;
			break;
		case 'L':
			headPosition += IVector2D::left;
			break;
		case 'R':
			headPosition += IVector2D::right;
			break;
		}
	}

	void UpdateTailPosition(const IVector2D& headPosition, IVector2D& tailPosition)
	{
		auto tailToHead{ headPosition - tailPosition };
		auto length{ tailToHead.Length() };

		if (length <= 1)
		{
			return;
		}
		
		check(length == 2);
		tailToHead.x = (tailToHead.x > 0) - (tailToHead.x < 0);
		tailToHead.y = (tailToHead.y > 0) - (tailToHead.y < 0);
		tailPosition += tailToHead;
	}
}

void Puzzle09::SolveA(const std::vector<std::string>& input) const
{
	using namespace Puzzle09Helpers;

	IVector2D headPosition{ IVector2D::zero };
	IVector2D tailPosition{ headPosition };

	// TODO: Implement hash function for Vector2D<T>.
	std::set<IVector2D> visitedPositions{ tailPosition };

	for (const auto& command : input)
	{
		auto direction{ command[0] };
		auto moves{ std::stoi(command.substr(2)) };

		while (moves-- > 0)
		{
			UpdateHeadPosition(direction, headPosition);
			UpdateTailPosition(headPosition, tailPosition);
			visitedPositions.emplace(tailPosition);
		}
	}

	std::cout << visitedPositions.size() << std::endl;
}

void Puzzle09::SolveB(const std::vector<std::string>& input) const
{
	using namespace Puzzle09Helpers;

	auto positions{ std::vector<IVector2D>{ 10 } };
	IVector2D& headPosition{ positions[0] };
	IVector2D& tailPosition{ positions[9] };
	std::set<IVector2D> visitedPositions{ tailPosition };

	for (const auto& command : input)
	{
		auto direction{ command[0] };
		auto moves{ std::stoi(command.substr(2)) };

		while (moves-- > 0)
		{
			UpdateHeadPosition(direction, headPosition);

			// Update the position of every knot after the head moves.
			for (auto index{ 0 }, next{ 1 }; next < 10; ++index, ++next)
			{
				UpdateTailPosition(positions[index], positions[next]);
			}
			visitedPositions.emplace(tailPosition);
		}
	}

	std::cout << visitedPositions.size() << std::endl;
}
