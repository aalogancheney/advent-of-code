#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle09Helpers
{
    struct Sequence
    {
        std::vector<int64> values{ };
        
        int64 GetNextValue() const
        {
            auto next{ [](const std::vector<int64>& input, int64 value) {
                return input.back() + value;
            }};
            return next(values, ReduceAndEvaluate(values, next));
        }

        int64 GetPreviousValue() const
        {
            auto previous{ [](const std::vector<int64>& input, int64 value) {
                return input.front() - value;
            } };
            return previous(values, ReduceAndEvaluate(values, previous));
        }

    private:
        int64 ReduceAndEvaluate(const std::vector<int64>& input, std::function<int64(const std::vector<int64>&, int64)> eval) const
        {
            // Determine whether or not reduction should continue in a single pass.
            bool bContinueReduction{ false };
            auto reduce{ [&bContinueReduction](int64 rhs, int64 lhs)
                {
                    int64 difference{ rhs - lhs };
                    bContinueReduction |= (difference != 0);
                    return difference;
                }};

            std::vector<int64> difference{ input };
            std::adjacent_difference(input.begin(), input.end(), difference.begin(), reduce);
            difference.erase(difference.begin()); // The front element is always the same as the input.
            return bContinueReduction ? eval(difference, ReduceAndEvaluate(difference, eval)) : 0;
        }
    };

    struct Oasis
    {
        std::vector<Sequence> sequences{ };

        int64 GetSumOfNextValues()
        {
            auto sumOfNextValues{ [](int64 value, Sequence sequence) { return sequence.GetNextValue() + value; } };
            return std::accumulate(sequences.begin(), sequences.end(), 0LL, sumOfNextValues);;
        }

        int64 GetSumOfPreviousValues()
        {
            auto sumOfPreviousValues{ [](int64 value, Sequence sequence) { return sequence.GetPreviousValue() + value; } };
            return std::accumulate(sequences.begin(), sequences.end(), 0LL, sumOfPreviousValues);
        }
    };

    auto CreateOasis(const std::vector<std::string>& input)
    {
        Oasis oasis{ };
        for (const auto& line : input)
        {
            oasis.sequences.emplace_back(Sequence{ });
            auto numbers{ Core::SplitString(line, " ") };
            for (const auto& number : numbers)
            {
                oasis.sequences.back().values.emplace_back(std::stoll(number));
            }
        }
        return oasis;
    }
}

void Puzzle09::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle09Helpers;

    auto oasis{ CreateOasis(input) };
    std::cout << oasis.GetSumOfNextValues() << std::endl;
}

void Puzzle09::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle09Helpers;

    auto oasis{ CreateOasis(input) };
    std::cout << oasis.GetSumOfPreviousValues() << std::endl;
}
