#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle05Helpers
{
    struct MappedRange : public Core::Math::Range<int64>
    {
        MappedRange(int64 inLowerBound, int64 inUpperBound, int64 inOffset)
            : Core::Math::Range<int64>{ inLowerBound, inUpperBound }
            , offset{ inOffset }
        {

        }

        int64 offset{ 0 };

        bool IsInMappedRange(int64 value) { return ContainsInclusive(value); }
        int64 Remap(int64 value) { check(IsInMappedRange(value)); return value + offset; }

        void Print() const
        {
            std::cout << "(" << GetLowerBound() << "," << GetUpperBound() << ":" << offset << ")";
        }

        //// Comparisons only care about bounds. The offset is used for remapping the values within the bounds.
        //auto operator<=>(const MappedRange& other) const { return Core::Math::Range<int64>::operator<=>(other); }
        //bool operator==(const MappedRange& other) const { return Core::Math::Range<int64>::operator==(other); }
    };

    struct Covering
    {
        std::vector<MappedRange> mappedRanges{ MappedRange{ 0, std::numeric_limits<int64>::max(), 0 } };

        void Print()
        {
            for (const auto& mappedRange : mappedRanges)
            {
                mappedRange.Print();
            }
        }

        auto MapValue(int64 value) const
        {
            for (const auto& mappedRange : mappedRanges)
            {
                if (mappedRange.ContainsInclusive(value))
                {
                    return value + mappedRange.offset;
                }
            }

            return 0LL;
            checkNoEntry();
        }

        void InsertMappedRange(const MappedRange& newMappedRange)
        {
            std::cout << "Inserting:\n\t";
            newMappedRange.Print();
            std::cout << "\nInto:\n\t";
            Print();

            // Find the index where this should be inserted.
            size_t index{ 0 };
            while (index < mappedRanges.size() && mappedRanges[index].GetLowerBound() < newMappedRange.GetLowerBound())
            {
                ++index;
            }

            mappedRanges.emplace(mappedRanges.begin() + index, newMappedRange);

            // Fix up things to the left.
            if (index > 0)
            {
                auto leftIndex{ index };
                auto& leftNeighbor{ mappedRanges[leftIndex - 1] };

                // Handle case where range needs to be split up. This can only happen from the left,
                // since insertion occurs in-order (so this isn't in the branch below).
                if (leftNeighbor.ContainsExclusive(newMappedRange))
                {
                    std::cout << "\nSplitting range";
                    mappedRanges[leftIndex].offset += leftNeighbor.offset;
                    auto upperSplit{ MappedRange{ newMappedRange.GetUpperBound() + 1, leftNeighbor.GetUpperBound(), leftNeighbor.offset } };
                    leftNeighbor.SetUpperBound(newMappedRange.GetLowerBound() - 1);
                    mappedRanges.emplace(mappedRanges.begin() + leftIndex + 1, upperSplit);
                    return;
                }

                // Erase values that are completely replaced by the new mapping.
                while (leftIndex > 0 && newMappedRange.ContainsInclusive(mappedRanges[leftIndex - 1]))
                {
                    std::cout << "\nErasing to the left";
                    mappedRanges[leftIndex - 1].offset += newMappedRange.offset;
                    //mappedRanges.erase(mappedRanges.begin() + leftIndex - 1);
                    --leftIndex;
                }

                if (leftIndex > 0 && newMappedRange.OverlapsUpper(mappedRanges[leftIndex - 1]))
                {
                    std::cout << "\nAdjusting bounds on the left";
                    mappedRanges[leftIndex - 1].SetUpperBound(newMappedRange.GetLowerBound() - 1);
                }
            }

            // Fix up things to the right.
            if (index < mappedRanges.size() - 1)
            {
                auto rightIndex{ index };
                auto& rightNeighbor{ mappedRanges[rightIndex + 1] };

                // Erase values that are completely replaced by the new mapping.
                while (rightIndex < mappedRanges.size() - 1 && newMappedRange.ContainsInclusive(mappedRanges[rightIndex + 1]))
                {
                    std::cout << "\nErasing to the right";
                    mappedRanges[rightIndex + 1].offset += newMappedRange.offset;
                    ++rightIndex;
                    //mappedRanges.erase(mappedRanges.begin() + rightIndex + 1);
                }

                if (rightIndex < mappedRanges.size() - 1 && newMappedRange.OverlapsLower(mappedRanges[rightIndex + 1]))
                {
                    std::cout << "\nAdjusting bounds on the right";
                    mappedRanges[rightIndex + 1].SetLowerBound(newMappedRange.GetUpperBound() + 1);
                }
            }

            std::cout << "\nResult:\n\t";
            Print();
            std::cout << "\n\n";
        }
    };

    struct Farm
    {
        std::vector<uint64> seeds{ };
        Covering covering{ };

        auto FindSmallestStartLocationA() const
        {
            int64 smallestValue{ std::numeric_limits<int64>::max() };
            for (const auto& seed : seeds)
            {
                auto mappedValue{ covering.MapValue(seed) };
                std::cout << "Mapped " << seed << " to " << mappedValue << "\n";
                smallestValue = std::min(smallestValue, mappedValue);
            }
            return smallestValue;
        }

        auto FindSmallestStartLocationB() const
        {

        }
    };

    auto CreateFarm(const std::vector<std::string>& input)
    {
        Farm farm{ };
        auto seeds{ Core::SplitString(Core::SplitString(input[0], "seeds: ")[1], " ") };
        for (const auto& seed : seeds)
        {
            farm.seeds.emplace_back(std::stoull(seed));
        }

        for (auto index{ 1 }; index < input.size(); ++index)
        {
            if (input[index] == "")
            {
                std::cout << "Inserting new mapping\n===\n";
                if (index > 12) { return farm; }
                ++index;
                continue;
            }

            auto rangeMappingValues{ Core::SplitString(input[index], " ") };
            check(rangeMappingValues.size() == 3);
            auto destinationRangeStart{ std::stoll(rangeMappingValues[0]) };
            auto sourceRangeStart{ std::stoll(rangeMappingValues[1]) };
            auto rangeLength{ std::stoll(rangeMappingValues[2]) };
            farm.covering.InsertMappedRange(MappedRange{ sourceRangeStart, sourceRangeStart + rangeLength - 1, destinationRangeStart - sourceRangeStart });
        }

        return farm;
    }

    /*struct RangeMapping
    {
        Core::Math::Range<uint64> mappedRange;
        uint64 destinationRangeStart{ 0 };

        RangeMapping(uint64 inDestinationRangeStart, uint64 sourceRangeStart, uint64 rangeLength)
            : mappedRange{ sourceRangeStart, sourceRangeStart + rangeLength - 1 }
            , destinationRangeStart{ inDestinationRangeStart}
        {

        }

        bool IsRemapped(uint64 input) const
        {
            return mappedRange.ContainsInclusive(input);
        }

        uint64 Map(uint64 input) const
        {
            check(IsRemapped(input));
            check(input >= mappedRange.GetLowerBound());
            return (input - mappedRange.GetLowerBound()) + destinationRangeStart;
        }
    };

    struct Mapping
    {
        std::vector<RangeMapping> rangeMappings{ };

        uint64 Map(uint64 input) const
        {
            for (const auto& rangeMapping : rangeMappings)
            {
                if (rangeMapping.IsRemapped(input))
                {
                    return rangeMapping.Map(input);
                }
            }
            return input;
        }
    };

    struct Farm
    {
        std::vector<uint64> seeds{ };
        std::vector<Mapping> mappings{ };

        uint64 GetLocationForSeed(uint64 seed) const
        {
            uint64 location{ seed };
            for (const auto& mapping : mappings)
            {
                location = mapping.Map(location);
            }
            return location;
        }

        uint64 FindSmallestStartLocationA() const
        {
            uint64 smallestStartLocation{ std::numeric_limits<uint64>::max() };
            for (const auto& seed : seeds)
            {
                smallestStartLocation = std::min(smallestStartLocation, GetLocationForSeed(seed));
            }
            return smallestStartLocation;
        }

        uint64 FindSmallestStartLocationB() const
        {
            uint64 smallestStartLocation{ std::numeric_limits<uint64>::max() };
            for (auto index{ 0 }; index < seeds.size(); index += 2)
            {
                const uint64 startSeed{ seeds[index] };
                const uint64 endSeed{ startSeed + seeds[static_cast<size_t>(1 + index)] };
                for (uint64 start{ startSeed }; start < endSeed; ++start)
                {
                    smallestStartLocation = std::min(smallestStartLocation, GetLocationForSeed(start));
                }
                std::cout << "Finished seed pair " << index / 2 << std::endl;
            }
            return smallestStartLocation;
        }
    };

    auto CreateFarm(const std::vector<std::string>& input)
    {
        Farm farm{ };
        auto seeds{ Core::SplitString(Core::SplitString(input[0], "seeds: ")[1], " ") };
        for (const auto& seed : seeds)
        {
            farm.seeds.emplace_back(std::stoull(seed));
        }

        for (auto index{ 1 }; index < input.size(); ++index)
        {
            if (input[index] == "")
            {
                farm.mappings.emplace_back(Mapping{ });
                ++index;
                continue;
            }

            auto rangeMappingValues{ Core::SplitString(input[index], " ") };
            check(rangeMappingValues.size() == 3);
            farm.mappings.back().rangeMappings.emplace_back(RangeMapping{
                std::stoull(rangeMappingValues[0]),
                std::stoull(rangeMappingValues[1]),
                std::stoull(rangeMappingValues[2]),
            });
        }

        return farm;
    }*/
}

void Puzzle05::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle05Helpers;

    std::cout << "\n";
    auto farm{ CreateFarm(input) };
    std::cout << "Final result:\n";
    farm.covering.Print();
    std::cout << "\n";

    std::cout << farm.FindSmallestStartLocationA() << std::endl;

    //std::cout << farm.FindSmallestStartLocationA() << std::endl;
}

void Puzzle05::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle05Helpers;

    //auto farm{ CreateFarm(input) };
    //std::cout << farm.FindSmallestStartLocationB() << std::endl;
    /*for (uint64 index{ 0 }; index < 100; ++index)
    {
        uint64 value{ index };
        std::cout << std::setw(3) << index;
        for (const auto& mapping : farm.mappings)
        {
            value = mapping.Map(value);
            std::cout << " >>> " << std::setw(3) << value;
        }
        std::cout << "\n";
    }*/
}
