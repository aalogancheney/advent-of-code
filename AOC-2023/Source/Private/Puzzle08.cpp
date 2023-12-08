#include "pch.h"

#include "Puzzles.h"
#include "Core.h"

#include <numeric>

namespace Puzzle08Helpers
{
    struct Node
    {
        std::string name{ "" };
        std::string left{ "" };
        std::string right{ "" };
    };

    struct Network
    {
        std::string leftRightInstructions{ };
        std::unordered_map<std::string, Node> nodes{ };
    };

    auto CreateNetwork(const std::vector<std::string> input)
    {
        Network network{ };

        network.leftRightInstructions = input[0];

        for (size_t index{ 2 }; index < input.size(); ++index)
        {
            auto nameToLeftRight{ Core::SplitString(input[index], " = ") };
            check(nameToLeftRight.size() == 2);
            network.nodes.emplace(nameToLeftRight[0], Node{ nameToLeftRight[0] });
        }

        for (size_t index{ 2 }; index < input.size(); ++index)
        {
            auto nameToLeftRight{ Core::SplitString(input[index], " = ") };
            check(nameToLeftRight.size() == 2);
            auto leftRight{ Core::SplitString(nameToLeftRight[1], ", ") };
            check(leftRight.size() == 2);
            auto leftName{ leftRight[0].substr(1, 3) };
            auto rightName{ leftRight[1].substr(0, 3) };
            network.nodes[nameToLeftRight[0]].left = leftName;
            network.nodes[nameToLeftRight[0]].right = rightName;
        }

        return network;
    }

    auto GetNumberOfStepsUntil(const Network& network, const Node* start, std::function<bool(const std::string& nodeName)> eval)
    {
        size_t steps{ 0 };
        auto* current{ start };
        while (!eval(current->name))
        {
            for (const auto& instruction : network.leftRightInstructions)
            {
                switch (instruction)
                {
                case 'L':
                    current = &network.nodes.at(current->left);
                    ++steps;
                    break;
                case 'R':
                    current = &network.nodes.at(current->right);
                    ++steps;
                    break;
                default:
                    checkNoEntry();
                    return steps;
                }

                if (eval(current->name)) { break; }
            }
        }
        return steps;
    }

    int64 LeastCommonMultiple(const std::vector<int64>& values)
    {
        if (values.size() == 0) { return 0; }
        if (values.size() == 1) { return values[0]; }
        if (values.size() == 2) { return std::lcm(values[0], values[1]); }
        int64 lcm{ std::lcm(values[0], values[1]) };
        for (size_t index{ 2 }; index < values.size(); ++index)
        {
            lcm = std::lcm(lcm, values[index]);
        }
        return lcm;
    }
}

void Puzzle08::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle08Helpers;

    const auto& network{ CreateNetwork(input) };
    std::cout << GetNumberOfStepsUntil(network, &network.nodes.at("AAA"), [](const std::string& nodeName) { return nodeName == "ZZZ"; }) << std::endl;
}

void Puzzle08::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle08Helpers;

    const auto& network{ CreateNetwork(input) };
    std::vector<int64> steps{ };
    for (const auto& [name, node] : network.nodes)
    {
        if (name[2] == 'A')
        {
            steps.emplace_back(GetNumberOfStepsUntil(network, &network.nodes.at(name), [](const std::string& nodeName)
            {
                return nodeName[2] == 'Z';
            }));
        }
    }
    std::cout << LeastCommonMultiple(steps) << std::endl;
}
