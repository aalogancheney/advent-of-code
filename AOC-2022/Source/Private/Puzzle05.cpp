#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle05Helpers
{
    class Cargo
    {
    public:
        Cargo(const std::vector<std::string>& input)
        {
            for (const auto& line : input)
            {
                if (line == "")
                {
                    break;
                }
                AddStack(line);
            }
        }

        void AddStack(const std::string& stack)
        {
            cargo.push_back(std::stack<char>{ });
            for (const auto& item : stack)
            {
                if (item != ' ')
                {
                    cargo.back().push(item);
                }
            }
        }

        void ExecuteMoveA(int32 count, int32 from, int32 to)
        {
            int32 fromIndex{ from - 1 };
            int32 toIndex{ to - 1 };

            check(from != to);
            check(fromIndex < cargo.size());
            check(toIndex < cargo.size());

            for (int32 i{ 0 }; i < count; ++i)
            {
                cargo[toIndex].push(cargo[fromIndex].top());
                cargo[fromIndex].pop();
            }
        }

        void ExecuteMoveB(int32 count, int32 from, int32 to)
        {
            int32 fromIndex{ from - 1 };
            int32 toIndex{ to - 1 };

            check(from != to);
            check(fromIndex < cargo.size());
            check(toIndex < cargo.size());

            std::stack<char> temp{ };
            for (int32 i{ 0 }; i < count; ++i)
            {
                temp.push(cargo[fromIndex].top());
                cargo[fromIndex].pop();
            }

            while (!temp.empty())
            {
                cargo[toIndex].push(temp.top());
                temp.pop();
            }
        }

        void PrintTop()
        {
            for (const auto& stack : cargo)
            {
                std::cout << stack.top();
            }
            std::cout << std::endl;
        }

    private:
        std::vector<std::stack<char>> cargo{ };
    };
}

void Puzzle05::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle05Helpers;

    Cargo cargo{ input };
    for (const auto& line : input)
    {
        if (!line.starts_with("move"))
        {
            continue;
        }

        std::stringstream ss{ line };
        std::string discard{ };
        int32 count, from, to;
        ss >> discard >> count >> discard >> from >> discard >> to;
        cargo.ExecuteMoveA(count, from, to);
    }
    cargo.PrintTop();
}

void Puzzle05::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle05Helpers;

    Cargo cargo{ input };
    for (const auto& line : input)
    {
        if (!line.starts_with("move"))
        {
            continue;
        }

        std::stringstream ss{ line };
        std::string discard{ };
        int32 count, from, to;
        ss >> discard >> count >> discard >> from >> discard >> to;
        cargo.ExecuteMoveB(count, from, to);
    }
    cargo.PrintTop();
}
