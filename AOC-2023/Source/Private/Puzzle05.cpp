#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle05Helpers
{
    struct RangeMapping
    {
        uint64 destinationRangeStart{ 0 };
        uint64 sourceRangeStart{ 0 };
        uint64 rangeLength{ 0 };

        bool IsRemapped(uint64 input) const
        {
            Core::Math::Range<uint64> mappedRange{ sourceRangeStart, sourceRangeStart + rangeLength };
            return mappedRange.IsInRange(input);
        }

        uint64 Map(uint64 input) const
        {
            if (!IsRemapped(input)) { return input; }
            check(input >= sourceRangeStart);
            return (input - sourceRangeStart) + destinationRangeStart;
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
                .destinationRangeStart = std::stoull(rangeMappingValues[0]),
                .sourceRangeStart = std::stoull(rangeMappingValues[1]),
                .rangeLength = std::stoull(rangeMappingValues[2]),
            });
        }

        return farm;
    }
}

void Puzzle05::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle05Helpers;

    auto farm{ CreateFarm(input) };
    //std::cout << farm.FindSmallestStartLocationA() << std::endl;
}

void Puzzle05::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle05Helpers;

    auto farm{ CreateFarm(input) };
    std::cout << farm.FindSmallestStartLocationB() << std::endl;
}
