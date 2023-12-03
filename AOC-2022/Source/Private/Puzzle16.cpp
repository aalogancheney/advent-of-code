#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle16Helpers
{
    // Each valve can be opened or closed. The flow rate, set of neighbors, and traversal costs
    // are immutable once the graph has been constructed.
    struct Valve
    {
        bool bOpened{ false };
        
        int32 flowRate{ };
        std::unordered_set<Valve*> neighbors{ };
        std::unordered_map<Valve*, int32> traversalCosts{ };
    };

    // A runner traverses the graph. Used for backtracking
    class Runner
    {
    public:
        Runner(Valve* startingValve, int32 initialTime)
        {
            path.emplace_back(std::make_pair(startingValve, initialTime));
        }

        // Returns true if the runner can reach the specified valve with the remaining time.
        auto CanReach(Valve* valve) const
        {
            return GetCurrentValve()->traversalCosts[valve] <= GetRemainingTime();
        }

        // Moves to the specified valve and returns the cost of traveling to that node.
        auto MoveToAndOpen(Valve* valve)
        {
            check(CanReach(valve));
            valve->bOpened = true;
            const int32 traversalCost{ GetCurrentValve()->traversalCosts[valve] };
            path.emplace_back(std::make_pair(valve, GetRemainingTime() - traversalCost));
            return traversalCost;
        }

        // Returns to the previous valve and resets the state of traversal.
        void Backtrack()
        {
            check(path.size() > 0);
            path.back().first->bOpened = false;
            path.pop_back();
        }

        // Returns a vector of all valves (with flow) that can be reached given the
        // remaining time.
        auto GetReachableUnopenedValves()
        {
            std::vector<Valve*> reachableUnopenedValves{ };
            for (const auto& [valve, cost] : GetCurrentValve()->traversalCosts)
            {
                if (!valve->bOpened && CanReach(valve))
                {
                    reachableUnopenedValves.emplace_back(valve);
                }
            }
            return reachableUnopenedValves;
        }

        // Get the current valve.
        Valve* GetCurrentValve() const
        {
            check(path.size() > 0);
            return path.back().first;
        }

        // Get the remaining time for the runner.
        int32 GetRemainingTime() const
        {
            check(path.size() > 0);
            return path.back().second;
        }

    private:
        // The path used by the runner. The back element is the current location.
        std::vector<std::pair<Valve*, int32>> path{ };
    };

    const std::string startNode{ "AA" };

    // Simple BFS to find shortest path to all other important nodes.
    void BuildTraversalCostsToImportantNodes(Valve& valve)
    {
        int32 cost{ 0 };
        std::unordered_set<Valve*> closedSet{ &valve };
        std::queue<Valve*> openSet{ };
        openSet.push(&valve);
        while (!openSet.empty())
        {
            ++cost;
            std::queue<Valve*> tempOpenSet{ };
            while (!openSet.empty())
            {
                Valve* neighbor{ openSet.front() };
                openSet.pop();

                // Only care of the neighbor has a positive flow rate.
                if (neighbor->flowRate > 0)
                {
                    valve.traversalCosts.emplace(neighbor, cost);
                }
                for (Valve* newNeighbors : neighbor->neighbors)
                {
                    if (!closedSet.contains(newNeighbors))
                    {
                        tempOpenSet.push(newNeighbors);
                    }
                }
                closedSet.emplace(neighbor);
            }
            openSet = tempOpenSet;
        }
    }

    // Precomputes the cost to get from important nodes to other important nodes.
    // This is useful because valves without flow only exist for traversal.
    void BuildTraversalCosts(std::unordered_map<std::string, Valve>& valves)
    {
        for (auto& [valveName, valve] : valves)
        {
            if (valveName == startNode || valve.flowRate > 0)
            {
                BuildTraversalCostsToImportantNodes(valve);
            }
        }
    }

    // Creates all of the valves from the provided input.
    std::unordered_map<std::string, Valve> CreateValves(const std::vector<std::string>& input)
    {
        std::vector<std::string> valveNamesAndFlows{ };
        std::vector<std::string> valvesNeighbors{ };
        for (const auto& line : input)
        {
            auto split{ Core::SplitString(line, ";") };
            valveNamesAndFlows.emplace_back(split[0]);
            valvesNeighbors.emplace_back(split[1]);
        }

        std::unordered_map<std::string, Valve> valves{ };

        // Build valves first, then add neighbors once all nodes have been created.
        for (const auto& valveNameAndFlow : valveNamesAndFlows)
        {
            auto name{ valveNameAndFlow.substr(6, 2) };
            valves.emplace(name, Valve{
                .flowRate = std::stoi(valveNameAndFlow.substr(23))
            });
        }
        for (auto index{ 0 }; index < valvesNeighbors.size(); ++index)
        {
            const auto sourceValve{ valveNamesAndFlows[index].substr(6, 2) };
            const auto neighbors{
                Core::SplitString(valvesNeighbors[index].size() > 25
                    ? valvesNeighbors[index].substr(24)
                    : valvesNeighbors[index].substr(23) // Silly edge case where the input uses correct grammar.
                , ", ")
            };
            for (const auto& neighbor : neighbors)
            {
                // Create two-way path between neighboring valves.
                valves.at(sourceValve).neighbors.emplace(&valves.at(neighbor));
                valves.at(neighbor).neighbors.emplace(&valves.at(sourceValve));
            }
        }

        // Populate the traversal costs now that nodes are created.
        BuildTraversalCosts(valves);
        return valves;
    }

    // Returns all pairs of valves that can be visited by two runners.
    std::vector<std::pair<Valve*, Valve*>> EnumerateValidPairs(const std::vector<Valve*>& lhs, const std::vector<Valve*>& rhs)
    {
        std::vector<std::pair<Valve*, Valve*>> enumeratedValidTargetPairs{ };
        for (const auto& firstTarget : lhs)
        {
            for (const auto& secondTarget : rhs)
            {
                if (firstTarget != secondTarget)
                {
                    enumeratedValidTargetPairs.emplace_back(std::make_pair(firstTarget, secondTarget));
                }
            }
        }
        return enumeratedValidTargetPairs;
    }

    // Finds maximum flow for a single runner.
    int32 CalculateMaxFlow(Runner& runner)
    {
        // Return 0 if there are no reachable valves.
        auto reachableUnopenedValves{ runner.GetReachableUnopenedValves() };
        if (reachableUnopenedValves.size() == 0) { return 0; }

        int32 maxFlow{ 0 };
        for (const auto& valve : reachableUnopenedValves)
        {
            runner.MoveToAndOpen(valve);
            const int32 addedFlow{ valve->flowRate * runner.GetRemainingTime() };
            maxFlow = std::max(maxFlow, addedFlow + CalculateMaxFlow(runner));
            runner.Backtrack();
        }
        return maxFlow;
    }

    int32 CalculateMaxFlow(Runner& human, Runner& elephant)
    {
        // If either runner has no more valves that can be reached, the problem degenerates into only needing a single runner.
        auto humanReachableUnopenedValves{ human.GetReachableUnopenedValves() };
        auto elephantReachableUnopenedValves{ elephant.GetReachableUnopenedValves() };
        if (humanReachableUnopenedValves.size() == 0) { return CalculateMaxFlow(elephant); }
        if (elephantReachableUnopenedValves.size() == 0) { return CalculateMaxFlow(human); }

        // Generate the list of valid pairs, given the set of reachable valves by both runners. If the resulting
        // set of pairs is empty, then both runners have exactly one reachable node in common, which means either
        // runner can open the valve.
        auto enumeratedValidPairs{ EnumerateValidPairs(humanReachableUnopenedValves, elephantReachableUnopenedValves) };
        if (enumeratedValidPairs.size() == 0) { return CalculateMaxFlow(human); }

        int32 maxFlow{ 0 };
        for (const auto& [humanTarget, elephantTarget] : enumeratedValidPairs)
        {
            int32 addedFlow{ 0 };

            human.MoveToAndOpen(humanTarget);
            addedFlow += humanTarget->flowRate * human.GetRemainingTime();

            elephant.MoveToAndOpen(elephantTarget);
            addedFlow += elephantTarget->flowRate * elephant.GetRemainingTime();

            maxFlow = std::max(maxFlow, addedFlow + CalculateMaxFlow(human, elephant));

            human.Backtrack();
            elephant.Backtrack();
        }
        return maxFlow;
    }
}

void Puzzle16::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle16Helpers;

    auto valves{ CreateValves(input) };
    Runner human{ &valves[startNode], 30 };
    auto maxFlow{ CalculateMaxFlow(human) };
    std::cout << maxFlow << std::endl;
}

void Puzzle16::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle16Helpers;

    auto valves{ CreateValves(input) };
    Runner human{ &valves[startNode], 26 };
    Runner elephant{ &valves[startNode], 26 };
    auto maxFlow{ CalculateMaxFlow(human, elephant) };
    std::cout << maxFlow << std::endl;
}
