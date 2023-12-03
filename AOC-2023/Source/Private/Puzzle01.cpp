#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle01Helpers
{
    static const std::unordered_map<std::string, int32> StringNumberToValueA
    {
        { "1",  1 },
        { "2",  2 },
        { "3",  3 },
        { "4",  4 },
        { "5",  5 },
        { "6",  6 },
        { "7",  7 },
        { "8",  8 },
        { "9",  9 },
    };

    static const std::unordered_map<std::string, int32> StringNumberToValueB
    {
        { "one",    1 },
        { "two",    2 },
        { "three",  3 },
        { "four",   4 },
        { "five",   5 },
        { "six",    6 },
        { "seven",  7 },
        { "eight",  8 },
        { "nine",   9 },
        { "1",      1 },
        { "2",      2 },
        { "3",      3 },
        { "4",      4 },
        { "5",      5 },
        { "6",      6 },
        { "7",      7 },
        { "8",      8 },
        { "9",      9 },
    };

    auto GetCalibrationValue(const std::string& line, const std::unordered_map<std::string, int32> StringNumberToValue)
    {
        struct IndexValue
        {
            size_t Index;
            int32 Value;
        };

        IndexValue FirstDigit{ std::string::npos, 0 };
        IndexValue LastDigit{ std::string::npos, 0 };

        for (const auto& [StringNumber, Value] : StringNumberToValue)
        {
            auto ForwardIndex{ line.find(StringNumber) };
            if (ForwardIndex != std::string::npos && (ForwardIndex < FirstDigit.Index || FirstDigit.Index == std::string::npos))
            {
                FirstDigit.Index = ForwardIndex;
                FirstDigit.Value = Value;
            }

            auto ReverseIndex{ line.rfind(StringNumber) };
            if (ReverseIndex != std::string::npos && (ReverseIndex > LastDigit.Index || LastDigit.Index == std::string::npos))
            {
                LastDigit.Index = ReverseIndex;
                LastDigit.Value = Value;
            }
        }

        return FirstDigit.Value * 10 + LastDigit.Value;
    }
}

void Puzzle01::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle01Helpers;

    int32 value{ 0 };
    for (const auto& line : input)
    {
        value += GetCalibrationValue(line, StringNumberToValueA);
    }
    std::cout << value << std::endl;
}

void Puzzle01::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle01Helpers;

    int32 value{ 0 };
    for (const auto& line : input)
    {
        value += GetCalibrationValue(line, StringNumberToValueB);
    }
    std::cout << value << std::endl;
}
