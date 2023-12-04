#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle04Helpers
{
    struct Card
    {
        int32 score{ 0 };
        int32 matchingNumbers{ 0 };

        Card(int32 inScore, int32 inMatchingNumbers)
            : score{ inScore }
            , matchingNumbers{ inMatchingNumbers }
        {

        }
    };

    auto CreateCard(const std::string& input)
    {
        constexpr auto cardDelimiter{ ": " };
        const auto& splitCard{ Core::SplitString(input, cardDelimiter) };
        check(splitCard.size() == 2);

        constexpr auto numbersDelimiter{ " | " };
        const auto& splitNumbers{ Core::SplitString(splitCard[1], numbersDelimiter) };
        check(splitNumbers.size() == 2);

        std::unordered_set<int32> winningNumbers{ };
        const auto& winningNumberStrings{ Core::SplitString(splitNumbers[0], " ") };
        for (const auto& winningNumber : winningNumberStrings)
        {
            if (winningNumber == "") { continue; }
            winningNumbers.emplace(std::stoi(winningNumber));
        }

        std::unordered_set<int32> numbersOnCard{ };
        const auto& numbersOnCardStrings{ Core::SplitString(splitNumbers[1], " ") };
        for (const auto& numberOnCard : numbersOnCardStrings)
        {
            if (numberOnCard == "") { continue; }
            numbersOnCard.emplace(std::stoi(numberOnCard));
        }

        // Pre-calculate score.
        int32 score{ 0 };
        for (const auto& winningNumber : winningNumbers)
        {
            if (numbersOnCard.contains(winningNumber))
            {
                if (score == 0) { score = 1; }
                else { score *= 2; }
            }
        }

        // Pre-calculate number of winning numbers.
        int32 matchingNumbers{ 0 };
        for (const auto& winningNumber : winningNumbers)
        {
            if (numbersOnCard.contains(winningNumber))
            {
                ++matchingNumbers;
            }
        }

        return Card{ score, matchingNumbers };
    }

    std::vector<Card> CreateCards(const std::vector<std::string>& input)
    {
        std::vector<Card> cards{ };
        for (const auto& line : input)
        {
            cards.emplace_back(CreateCard(line));
        }
        return cards;
    }

    int32 ProcessCopies(const std::vector<Card>& cards, size_t startIndex, size_t endIndex)
    {
        check(endIndex >= startIndex);

        if (startIndex >= cards.size()) { return 0; }
        int32 totalNumberOfCards{ static_cast<int32>(endIndex - startIndex) };
        for (auto index{ startIndex }; index < endIndex; ++index)
        {
            totalNumberOfCards += ProcessCopies(cards, index + 1, index + 1 + cards[index].matchingNumbers);
        }
        return totalNumberOfCards;
    }
}

void Puzzle04::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle04Helpers;

    auto cards{ CreateCards(input) };
    auto totalScore{ 0 };
    for (const auto& card : cards)
    {
        totalScore += card.score;
    }
    std::cout << totalScore << std::endl;;
}

void Puzzle04::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle04Helpers;

    auto cards{ CreateCards(input) };
    auto totalCopies{ ProcessCopies(cards, 0, cards.size()) };
    std::cout << totalCopies << std::endl;
}
