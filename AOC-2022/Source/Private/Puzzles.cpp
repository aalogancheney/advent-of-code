#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

void Puzzle01::SolveA(const std::vector<std::string>& input) const
{
	std::vector<int32> caloriesForElf{ };
	caloriesForElf.emplace_back(0);
	for (const auto& snack : input)
	{
		if (snack == "")
		{
			caloriesForElf.emplace_back(0);
		}
		else
		{
			const int32 calories{ std::stoi(snack) };
			caloriesForElf.back() += calories;
		}
	}

	std::sort(caloriesForElf.begin(), caloriesForElf.end(), std::greater<int32>());
	std::cout << caloriesForElf.front() << std::endl;
}

void Puzzle01::SolveB(const std::vector<std::string>& input) const
{
	std::vector<int32> caloriesForElf{ };
	caloriesForElf.emplace_back(0);
	for (const auto& snack : input)
	{
		if (snack == "")
		{
			caloriesForElf.emplace_back(0);
		}
		else
		{
			const int32 calories{ std::stoi(snack) };
			caloriesForElf.back() += calories;
		}
	}

	std::sort(caloriesForElf.begin(), caloriesForElf.end(), std::greater<int32>());
	const int32 topThreeElfCalories{ caloriesForElf[0] + caloriesForElf[1] + caloriesForElf[2] };
	std::cout << topThreeElfCalories << std::endl;
}

DEFINE_EMPTY_PUZZLE(02)
DEFINE_EMPTY_PUZZLE(03)
DEFINE_EMPTY_PUZZLE(04)
DEFINE_EMPTY_PUZZLE(05)
DEFINE_EMPTY_PUZZLE(06)
DEFINE_EMPTY_PUZZLE(07)
DEFINE_EMPTY_PUZZLE(08)
DEFINE_EMPTY_PUZZLE(09)
DEFINE_EMPTY_PUZZLE(10)
DEFINE_EMPTY_PUZZLE(11)
DEFINE_EMPTY_PUZZLE(12)
DEFINE_EMPTY_PUZZLE(13)
DEFINE_EMPTY_PUZZLE(14)
DEFINE_EMPTY_PUZZLE(15)
DEFINE_EMPTY_PUZZLE(16)
DEFINE_EMPTY_PUZZLE(17)
DEFINE_EMPTY_PUZZLE(18)
DEFINE_EMPTY_PUZZLE(19)
DEFINE_EMPTY_PUZZLE(20)
DEFINE_EMPTY_PUZZLE(21)
DEFINE_EMPTY_PUZZLE(22)
DEFINE_EMPTY_PUZZLE(23)
DEFINE_EMPTY_PUZZLE(24)
DEFINE_EMPTY_PUZZLE(25)
