#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle04Helpers
{
    struct Card
    {
        int32 score{ 0 };
        int32 matchingNumbers{ 0 };
        std::unordered_set<int32> winningNumbers{ };
        std::unordered_set<int32> numbersOnCard{ };

        auto Init()
        {
            // Pre-calculate score.
            for (const auto& winningNumber : winningNumbers)
            {
                if (numbersOnCard.contains(winningNumber))
                {
                    if (score == 0) { score = 1; }
                    else { score *= 2; }
                }
            }

            // Pre-calculate number of winning numbers.
            for (const auto& winningNumber : winningNumbers)
            {
                if (numbersOnCard.contains(winningNumber))
                {
                    ++matchingNumbers;
                }
            }
        }
    };

    auto CreateCard(const std::string& input)
    {
        Card card{ };

        constexpr auto cardDelimiter{ ": " };
        const auto& splitCard{ Core::SplitString(input, cardDelimiter) };
        check(splitCard.size() == 2);

        constexpr auto numbersDelimiter{ " | " };
        const auto& splitNumbers{ Core::SplitString(splitCard[1], numbersDelimiter) };
        check(splitNumbers.size() == 2);

        const auto& winningNumbers{ Core::SplitString(splitNumbers[0], " ") };
        for (const auto& winningNumber : winningNumbers)
        {
            if (winningNumber == "") { continue; }
            card.winningNumbers.emplace(std::stoi(winningNumber));
        }

        const auto& numbersOnCard{ Core::SplitString(splitNumbers[1], " ") };
        for (const auto& numberOnCard : numbersOnCard)
        {
            if (numberOnCard == "") { continue; }
            card.numbersOnCard.emplace(std::stoi(numberOnCard));
        }

        card.Init();

        return card;
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
