#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle07Helpers
{
    // Ordered hand types according to rankings (lower values are lower scoring).
    enum class HandType
    {
        HighCard,       // 5
        OnePair,        // 4
        TwoPair,        // 3
        ThreeOfAKind,   // 3
        FullHouse,      // 2
        FourOfAKind,    // 2
        FiveOfAKind,    // 1
    };

    constexpr uint8 CardToValueA(char c)
    {
        switch (c)
        {
        case 'A': return 14;
        case 'K': return 13;
        case 'Q': return 12;
        case 'J': return 11;
        case 'T': return 10;
        default: return c - '0';
        }
    }

    // Put jokers at the bottom of the value.
    constexpr uint8 CardToValueB(char c)
    {
        switch (c)
        {
        case 'A': return 14;
        case 'K': return 13;
        case 'Q': return 12;
        case 'J': return 1;
        case 'T': return 10;
        default: return c - '0';
        }
    }
    
    // Calculate the hand type based on the values.
    HandType CalculateHandTypeA(uint8 handValues[5])
    {
        std::unordered_map<uint8, uint8> handValueCounts{ };
        for (auto index{ 0 }; index < 5; ++index)
        {
            auto value{ handValues[index] };
            if (!handValueCounts.contains(value))
            {
                handValueCounts[value] = 0;
            }
            handValueCounts[value]++;
        }

        switch (handValueCounts.size())
        {
        case 1: return HandType::FiveOfAKind;
        case 4: return HandType::OnePair;
        case 5: return HandType::HighCard;
        case 2:
            for (const auto& [card, count] : handValueCounts)
            {
                if (count == 4) { return HandType::FourOfAKind; }
                if (count == 3) { return HandType::FullHouse; }
            }
        case 3:
            for (const auto& [card, count] : handValueCounts)
            {
                if (count == 3) { return HandType::ThreeOfAKind; }
                if (count == 2) { return HandType::TwoPair; }
            }
        default: checkNoEntry();
        }
    }

    // Calculate the hand type based on the values. Jokers are wild, so they're
    // used to improve the hand as much as possible.
    HandType CalculateHandTypeB(uint8 handValues[5])
    {
        std::unordered_map<uint8, uint8> handValueCounts{ };
        for (auto index{ 0 }; index < 5; ++index)
        {
            auto value{ handValues[index] };
            if (!handValueCounts.contains(value))
            {
                handValueCounts[value] = 0;
            }
            handValueCounts[value]++;
        }

        // The number of jokers will determine how much the hand can be improved.
        auto jokerCount{ 0 };
        if (handValueCounts.contains(1))
        {
            jokerCount = handValueCounts[1];
        }

        switch (handValueCounts.size())
        {
        case 1:
            // Can't be improved.
            check(jokerCount == 0 || jokerCount == 5);
            return HandType::FiveOfAKind;

        case 4:
            // One pair.
            switch (jokerCount)
            {
            case 0: return HandType::OnePair;
            case 1:
            case 2:
                return HandType::ThreeOfAKind;
            default:
                checkNoEntry();
            }

        case 5:
            // High card.
            switch (jokerCount)
            {
            case 0: return HandType::HighCard;
            case 1: return HandType::OnePair;
            default: checkNoEntry();
            }

        case 2:
            // Four of a kind or a full house.
            for (const auto& [card, count] : handValueCounts)
            {
                // Four of a kind.
                if (count == 4)
                {
                    switch (jokerCount)
                    {
                    case 0: return HandType::FourOfAKind;
                    case 1:
                    case 4:
                        return HandType::FiveOfAKind;
                    default:
                        checkNoEntry();
                    }
                }

                // Full house.
                if (count == 3)
                {
                    switch (jokerCount)
                    {
                    case 0: return HandType::FullHouse;
                    case 2:
                    case 3:
                        return HandType::FiveOfAKind;
                    default:
                        checkNoEntry();
                    }
                }
            }

        case 3:
            // Three of a kind or two pair.
            for (const auto& [card, count] : handValueCounts)
            {
                // Three of a kind.
                if (count == 3)
                {
                    switch (jokerCount)
                    {
                    case 0: return HandType::ThreeOfAKind;
                    case 3:
                    case 1:
                        return HandType::FourOfAKind;
                    default:
                        checkNoEntry();
                    }
                }

                // Two pair.
                if (count == 2)
                {
                    switch (jokerCount)
                    {
                    case 0: return HandType::TwoPair;
                    case 1: return HandType::FullHouse;
                    case 2: return HandType::FourOfAKind;
                    default:
                        checkNoEntry();
                    }
                }
            }
        default: checkNoEntry();
        }
    }

    struct Hand
    {
        HandType handType{ };
        uint8 handValues[5]{ };
        int32 bid{ };

        auto operator<(const Hand& other) const
        {
            if (handType != other.handType)
            {
                return handType < other.handType;
            }
            for (auto index{ 0 }; index < 5; ++index)
            {
                if (handValues[index] != other.handValues[index])
                {
                    return handValues[index] < other.handValues[index];
                }
            }
        }
    };
}

void Puzzle07::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle07Helpers;

    std::vector<Hand> hands{ };
    for (const auto& line : input)
    {
        const auto& cardBid{ Core::SplitString(line, " ")};
        check(cardBid.size() == 2);
        Hand hand{ };
        hand.bid = std::stoi(cardBid[1]);
        for (auto index{ 0 }; index < cardBid[0].size(); ++index)
        {
            hand.handValues[index] = CardToValueA(cardBid[0][index]);
        }
        hand.handType = CalculateHandTypeA(hand.handValues);
        hands.emplace_back(hand);
    }
    std::sort(hands.begin(), hands.end());
    
    uint64 score{ 0 };
    uint64 index{ 1 };
    for (const auto& hand : hands)
    {
        score += hand.bid * index++;
    }
    std::cout << score << std::endl;
}

void Puzzle07::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle07Helpers;

    std::vector<Hand> hands{ };
    for (const auto& line : input)
    {
        const auto& cardBid{ Core::SplitString(line, " ") };
        check(cardBid.size() == 2);
        Hand hand{ };
        hand.bid = std::stoi(cardBid[1]);
        for (auto index{ 0 }; index < cardBid[0].size(); ++index)
        {
            hand.handValues[index] = CardToValueB(cardBid[0][index]);
        }
        hand.handType = CalculateHandTypeB(hand.handValues);
        hands.emplace_back(hand);
    }
    std::sort(hands.begin(), hands.end());

    uint64 score{ 0 };
    uint64 index{ 1 };
    for (const auto& hand : hands)
    {
        score += hand.bid * index++;
    }
    std::cout << score << std::endl;
}
