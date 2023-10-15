#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle01Helpers
{
	std::vector<int32> GetTotalCaloriesPerElf(const std::vector<std::string>& input)
	{
		std::vector<int32> caloriesPerElf{ };
		caloriesPerElf.emplace_back(0);
		for (const auto& snack : input)
		{
			if (snack == "")
			{
				caloriesPerElf.emplace_back(0);
			}
			else
			{
				const int32 calories{ std::stoi(snack) };
				caloriesPerElf.back() += calories;
			}
		}

		std::sort(caloriesPerElf.begin(), caloriesPerElf.end(), std::greater<int32>());
		return caloriesPerElf;
	}
}

void Puzzle01::SolveA(const std::vector<std::string>& input) const
{
	auto caloriesPerElf{ Puzzle01Helpers::GetTotalCaloriesPerElf(input) };
	check(caloriesPerElf.size() >= 1)
	std::cout << caloriesPerElf.front() << std::endl;
}

void Puzzle01::SolveB(const std::vector<std::string>& input) const
{
	auto caloriesPerElf{ Puzzle01Helpers::GetTotalCaloriesPerElf(input) };
	check(caloriesPerElf.size() >= 3);
	const int32 topThreeElfCalories{ caloriesPerElf[0] + caloriesPerElf[1] + caloriesPerElf[2] };
	std::cout << topThreeElfCalories << std::endl;
}
