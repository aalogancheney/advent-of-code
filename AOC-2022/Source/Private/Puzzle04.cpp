#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

using Range = Core::Math::Range<int32>;

namespace Puzzle04Helpers
{
	std::vector<std::string> GetStringRanges(const std::string& stringRanges)
	{
		static const std::string delimiter{ "," };
		return Core::SplitString(stringRanges, delimiter);
	}

	Range ConstructRange(const std::string& stringRange)
	{
		static const std::string delimiter{ "-" };
		std::vector<std::string> rangeNumbers{ Core::SplitString(stringRange, delimiter) };
		check(rangeNumbers.size() == 2);
		return Range{ std::stoi(rangeNumbers[0]), std::stoi(rangeNumbers[1]) };
	}
}

void Puzzle04::SolveA(const std::vector<std::string>& input) const
{
	using namespace Puzzle04Helpers;

	int32 totalOverlapCount{ 0 };
	for (const auto& pairs : input)
	{
		auto stringRanges{ GetStringRanges(pairs) };
		check(stringRanges.size() == 2);
		Range firstRange{ ConstructRange(stringRanges[0]) };
		Range secondRange{ ConstructRange(stringRanges[1]) };
		if (firstRange.IsContainedBy(secondRange) || secondRange.IsContainedBy(firstRange))
		{
			++totalOverlapCount;
		}
	}
	std::cout << totalOverlapCount << std::endl;
}

void Puzzle04::SolveB(const std::vector<std::string>& input) const
{
	using namespace Puzzle04Helpers;

	int32 totalOverlapCount{ 0 };
	for (const auto& pairs : input)
	{
		auto stringRanges{ GetStringRanges(pairs) };
		check(stringRanges.size() == 2);
		Range firstRange{ ConstructRange(stringRanges[0]) };
		Range secondRange{ ConstructRange(stringRanges[1]) };
		if (firstRange.Overlaps(secondRange) || secondRange.Overlaps(firstRange))
		{
			++totalOverlapCount;
		}
	}
	std::cout << totalOverlapCount << std::endl;
}
