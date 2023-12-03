#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle02Helpers
{
    struct Result
    {
        int32 red{ 0 };
        int32 blue{ 0 };
        int32 green{ 0 };
    };

    struct Game
    {
        int32 id{ 0 };
        std::vector<Result> results{ };
    };

    static const auto gameDelimiter{ ": " };
    static const auto resultsDelimiter{ "; " };
    static const auto cubeDelimiter{ ", " };
    static const auto red{ "red" };
    static const auto blue{ "blue" };
    static const auto green{ "green" };

    auto ParseGame(const std::string& line)
    {
        Game game{ };
        auto gameToResults{ Core::SplitString(line, gameDelimiter) };
        check(gameToResults.size() == 2);

        game.id = std::stoi(gameToResults[0].substr(std::string{ "Game " }.length()));

        auto results{ Core::SplitString(gameToResults[1], resultsDelimiter) };
        for (const auto& result : results)
        {
            auto cubes{ Core::SplitString(result, cubeDelimiter) };
            game.results.push_back({ });
            for (const auto& cube : cubes)
            {
                auto numberToColor{ Core::SplitString(cube, " ") };
                check(numberToColor.size() == 2);
                auto number{ std::stoi(numberToColor[0]) };

                if      (numberToColor[1] == red)   { game.results.back().red += number;    }
                else if (numberToColor[1] == blue)  { game.results.back().blue += number;   }
                else if (numberToColor[1] == green) { game.results.back().green += number;  }
                else                                { checkNoEntry();                       }
            }
        }

        return game;
    }

    static auto IsGameValid(const Game& game, const Result& maximalResult)
    {
        bool bIsValid{ true };
        for (const auto& result : game.results)
        {
            bIsValid &= result.red <= maximalResult.red && result.blue <= maximalResult.blue && result.green <= maximalResult.green;
        }
        return bIsValid;
    }

    static auto CalculatePower(const Game& game)
    {
        Result minimumResult{ };
        for (const auto& result : game.results)
        {
            minimumResult.red = std::max(minimumResult.red, result.red);
            minimumResult.blue = std::max(minimumResult.blue, result.blue);
            minimumResult.green = std::max(minimumResult.green, result.green);
        }
        return minimumResult.red * minimumResult.blue * minimumResult.green;
    }
}

void Puzzle02::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle02Helpers;

    static Result maximalResult{
        .red{ 12 },
        .blue{ 14 },
        .green{ 13 },
    };

    auto sumOfValidGameIds{ 0 };
    for (const auto& line : input)
    {
        auto game{ ParseGame(line) };
        if (IsGameValid(game, maximalResult))
        {
            sumOfValidGameIds += game.id;
        }
    }
    std::cout << sumOfValidGameIds << std::endl;
}

void Puzzle02::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle02Helpers;

    auto sumOfPower{ 0 };
    for (const auto& line : input)
    {
        auto game{ ParseGame(line) };
        sumOfPower += CalculatePower(game);
    }
    std::cout << sumOfPower << std::endl;
}
