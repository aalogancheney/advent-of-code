#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle06Helpers
{
    struct Race
    {
        uint64 time{ 0 };
        uint64 distance{ 0 };
    };
}

void Puzzle06::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle06Helpers;

    std::vector<Race> races
    {
        { 60, 601  },
        { 80, 1163 },
        { 86, 1559 },
        { 76, 1300 },
    };

    uint64 result{ 1 };
    for (Race race : races)
    {
        uint64 invalidRaces{ 0 };
        while ((race.time - invalidRaces) * invalidRaces <= race.distance) { ++invalidRaces; }
        invalidRaces *= 2;
        result *= race.time - invalidRaces + 1;
    }
    std::cout << result << std::endl;
}

void Puzzle06::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle06Helpers;

    std::vector<uint64> waysToWin{ };
    Race race{ 60808676, 601116315591300 };
    uint64 invalidRaces{ 0 };
    while ((race.time - invalidRaces) * invalidRaces <= race.distance) { ++invalidRaces; }
    invalidRaces *= 2;
    std::cout << race.time - invalidRaces + 1 << std::endl;
}
