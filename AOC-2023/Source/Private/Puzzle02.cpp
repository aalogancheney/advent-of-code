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

    Game ParseGame(const std::string& line)
    {
        Game game{ };
        std::vector<std::string> gameToResults{ Core::SplitString(line, ": ") };
        check(gameToResults.size() == 2);

        game.id = std::stoi(gameToResults[0].substr(std::string{ "Game " }.length()));

        std::vector<std::string> results{ Core::SplitString(gameToResults[1], "; ") };
        for (const auto& result : results)
        {
            std::vector<std::string> cubes{ Core::SplitString(result, ", ") };
            game.results.push_back({ });
            for (const auto& cube : cubes)
            {
                std::vector<std::string> numberToColor{ Core::SplitString(cube, " ") };
                check(numberToColor.size() == 2);
                int32 number{ std::stoi(numberToColor[0]) };
                if (numberToColor[1] == "red")
                {
                    game.results.back().red += number;
                }
                else if (numberToColor[1] == "blue")
                {
                    game.results.back().blue += number;
                }
                else if (numberToColor[1] == "green")
                {
                    game.results.back().green += number;
                }
                else
                {
                    checkNoEntry();
                }
            }
        }

        return game;
    }

    static bool IsGameValid(const Game& game, const Result& maximalResult)
    {
        bool bIsValid{ true };
        for (const auto& result : game.results)
        {
            bIsValid &= result.red <= maximalResult.red && result.blue <= maximalResult.blue && result.green <= maximalResult.green;
        }
        return bIsValid;
    }

    static int32 CalculatePower(const Game& game)
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

    int32 sumOfValidGameIds{ 0 };
    for (const auto& line : input)
    {
        Game game{ ParseGame(line) };
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

    int32 sumOfPower{ 0 };
    for (const auto& line : input)
    {
        Game game{ ParseGame(line) };
        sumOfPower += CalculatePower(game);
    }
    std::cout << sumOfPower << std::endl;
}
