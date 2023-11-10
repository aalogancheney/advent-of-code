#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle11Helpers
{
    class Monkey
    {
    public:
        void Initialize(std::initializer_list<uint64> inItems, std::function<uint64(uint64)> inOperation, std::function<bool(uint64)> inTest, Monkey* inTrueTarget, Monkey* inFalseTarget)
        {
            for (auto item : inItems)
            {
                items.emplace(item);
            }
            operation = inOperation;
            test = inTest;
            trueTarget = inTrueTarget;
            falseTarget = inFalseTarget;
        }

        void TakeTurn(uint64 boredomFactor, bool modulus)
        {
            while (!items.empty())
            {
                ++totalInspections;

                uint64 item{ items.front() };
                items.pop();
                item = operation(item);
                item /= boredomFactor;
                if (modulus)
                {
                    item %= lcm;
                }

                if (test(item))
                {
                    trueTarget->Catch(item);
                }
                else
                {
                    falseTarget->Catch(item);
                }
            }
        }

        uint64 GetTotalInspections() const { return totalInspections; }

    private:
        void Catch(uint64 item) { items.emplace(item); }

        uint64 totalInspections{ 0 };
        static constexpr uint64 lcm{ 2 * 3 * 5 * 7 * 11 * 13 * 17 * 19 };

        std::queue<uint64> items{ };
        std::function<uint64(uint64)> operation;
        std::function<bool(uint64)> test;
        Monkey* trueTarget{ nullptr };
        Monkey* falseTarget{ nullptr };
    };

    std::vector<Monkey> InitializeMonkeys()
    {
        static constexpr int32 numberOfMonkeys{ 8 };
        
        std::vector<Monkey> monkeys{ };
        for (int32 count{ 0 }; count < numberOfMonkeys; ++count)
        {
            monkeys.emplace_back(Monkey{ });
        }

        monkeys.at(0).Initialize({ 54, 98, 50, 94, 69, 62, 53, 85 },
            [](uint64 value) { return value * 13; },
            [](uint64 value) { return (value % 3) == 0; },
            &monkeys.at(2), &monkeys.at(1));

        monkeys.at(1).Initialize({ 71, 55, 82 },
            [](uint64 value) { return value + 2; },
            [](uint64 value) { return (value % 13) == 0; },
            &monkeys.at(7), &monkeys.at(2));

        monkeys.at(2).Initialize({ 77, 73, 86, 72, 87 },
            [](uint64 value) { return value + 8; },
            [](uint64 value) { return (value % 19) == 0; },
            &monkeys.at(4), &monkeys.at(7));

        monkeys.at(3).Initialize({ 97, 91 },
            [](uint64 value) { return value + 1; },
            [](uint64 value) { return (value % 17) == 0; },
            &monkeys.at(6), &monkeys.at(5));

        monkeys.at(4).Initialize({ 78, 97, 51, 85, 66, 63, 62 },
            [](uint64 value) { return value * 17; },
            [](uint64 value) { return (value % 5) == 0; },
            &monkeys.at(6), &monkeys.at(3));

        monkeys.at(5).Initialize({ 88 },
            [](uint64 value) { return value + 3; },
            [](uint64 value) { return (value % 7) == 0; },
            &monkeys.at(1), &monkeys.at(0));

        monkeys.at(6).Initialize({ 87, 57, 63, 86, 87, 53 },
            [](uint64 value) { return value * value; },
            [](uint64 value) { return (value % 11) == 0; },
            &monkeys.at(5), &monkeys.at(0));

        monkeys.at(7).Initialize({ 73, 59, 82, 65 },
            [](uint64 value) { return value + 6; },
            [](uint64 value) { return (value % 2) == 0; },
            &monkeys.at(4), &monkeys.at(3));

        return monkeys;
    }

    void RunSimulation(uint32 rounds, uint64 boredomFactor, bool modulus)
    {
        std::vector<Monkey> monkeys{ InitializeMonkeys() };

        for (uint32 count{ 0 }; count < rounds; ++count)
        {
            for (auto& monkey : monkeys)
            {
                monkey.TakeTurn(boredomFactor, modulus);
            }
        }

        auto sorter = [](const Monkey& lhs, const Monkey& rhs) { return lhs.GetTotalInspections() > rhs.GetTotalInspections(); };
        std::sort(monkeys.begin(), monkeys.end(), sorter);
        std::cout << monkeys.at(0).GetTotalInspections() * monkeys.at(1).GetTotalInspections() << std::endl;
    }
}

void Puzzle11::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle11Helpers;

    RunSimulation(20, 3, false);
}

void Puzzle11::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle11Helpers;

    RunSimulation(10000, 1, true);
}
