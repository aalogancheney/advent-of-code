#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle03Helpers
{
	struct Bits
	{
		Bits()
		{
			for (int32 index{ 0 }; index < 64; ++index)
			{
				bits[index] = (uint64)1 << index;
			}
		}

		int32 GetBitIndex(uint64 input)
		{
			for (int32 index{ 0 }; index < 64; ++index)
			{
				if (bits[index] == input)
				{
					return index;
				}
			}
			checkNoEntry();
			return -1;
		}

	private:
		uint64 bits[64] { 0 };
	};

	int32 GetItemIndexA(char item)
	{
		check(std::isupper(item) || std::islower(item));
		if (std::islower(item))
		{
			return item - 'a';
		}
		return item - 'A' + 26;
	}
	
	int32 FindOverlappingItemPriorityA(const std::string& rucksack)
	{
		static Bits bits{ };

		check(rucksack.size() % 2 == 0);
		const auto compartmentSize{ rucksack.size() / 2 };

		const std::string& firstCompartment{ rucksack.substr(0, compartmentSize) };
		const std::string& secondCompartment{ rucksack.substr(compartmentSize) };

		uint64 uniqueItemsInFirstCompartment{ 0 };
		for (const auto& item : firstCompartment)
		{
			uniqueItemsInFirstCompartment |= (uint64)1 << GetItemIndexA(item);
		}

		uint64 uniqueItemsInSecondCompartment{ 0 };
		for (const auto& item : secondCompartment)
		{
			uniqueItemsInSecondCompartment |= (uint64)1 << GetItemIndexA(item);
		}

		const uint64 overlap{ uniqueItemsInFirstCompartment & uniqueItemsInSecondCompartment };
		check((overlap & (overlap - 1)) == 0);
		int32 bitIndex{ bits.GetBitIndex(overlap) };
		return bitIndex + 1;
	}

	int32 FindOverlappingItemPriorityB(const std::string& rucksack1, const std::string& rucksack2, const std::string& rucksack3)
	{
		static Bits bits{ };

		uint64 uniqueItemsInRucksack1{ 0 };
		for (const auto& item : rucksack1)
		{
			uniqueItemsInRucksack1 |= (uint64)1 << GetItemIndexA(item);
		}

		uint64 uniqueItemsInRucksack2{ 0 };
		for (const auto& item : rucksack2)
		{
			uniqueItemsInRucksack2 |= (uint64)1 << GetItemIndexA(item);
		}

		uint64 uniqueItemsInRucksack3{ 0 };
		for (const auto& item : rucksack3)
		{
			uniqueItemsInRucksack3 |= (uint64)1 << GetItemIndexA(item);
		}

		const uint64 overlap{ uniqueItemsInRucksack1 & uniqueItemsInRucksack2 & uniqueItemsInRucksack3 };
		check((overlap & (overlap - 1)) == 0);
		int32 bitIndex{ bits.GetBitIndex(overlap) };
		return bitIndex + 1;
	}
}

void Puzzle03::SolveA(const std::vector<std::string>& input) const
{
	using namespace Puzzle03Helpers;
	
	auto priority{ 0 };
	for (const auto& rucksack : input)
	{
		priority += FindOverlappingItemPriorityA(rucksack);
	}
	std::cout << priority << std::endl;
}

void Puzzle03::SolveB(const std::vector<std::string>& input) const
{
	using namespace Puzzle03Helpers;

	check(input.size() % 3 == 0);

	auto priority{ 0 };
	for (int32 index{ 0 }; index < input.size(); index += 3)
	{
		priority += FindOverlappingItemPriorityB(input[index], input[index + 1], input[index + 2]);
	}
	std::cout << priority << std::endl;
}
