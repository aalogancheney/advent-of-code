#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

#include "Puzzle13/ValueItem.h"
#include "Puzzle13/ListItem.h"

namespace Puzzle13Helpers
{
    static ListItemPtr ToList(ItemPtr item) { return std::dynamic_pointer_cast<ListItem>(item); }
    static ValueItemPtr ToValue(ItemPtr item) { return std::dynamic_pointer_cast<ValueItem>(item); }

    int32 Compare(const ItemPtr left, const ItemPtr right)
    {
        check(left);
        check(right);

        if (left->GetItemType() == right->GetItemType() && left->GetItemType() == EItemType::Value)
        {
            ValueItemPtr leftValue{ ToValue(left) };
            ValueItemPtr rightValue{ ToValue(right) };
            check(leftValue);
            check(rightValue);
            if (leftValue->value < rightValue->value) { return -1; }
            if (leftValue->value == rightValue->value) { return 0; }
            return 1;
        }

        if (left->GetItemType() == right->GetItemType() && left->GetItemType() == EItemType::List)
        {
            ListItemPtr leftList{ ToList(left) };
            ListItemPtr rightList{ ToList(right) };
            check(leftList);
            check(rightList);

            for (size_t leftIndex{ 0 }, rightIndex{ 0 }; leftIndex < leftList->items.size() && rightIndex < rightList->items.size(); ++leftIndex, ++rightIndex)
            {
                int32 comparison{ Compare(leftList->items[leftIndex], rightList->items[rightIndex]) };
                if (comparison != 0)
                {
                    return comparison;
                }
            }

            if (leftList->items.size() < rightList->items.size()) { return -1; }
            if (leftList->items.size() == rightList->items.size()) { return 0; }
            return 1;
        }

        if (left->GetItemType() == EItemType::Value)
        {
            ValueItemPtr leftValue{ ToValue(left) };
            return Compare(leftValue->ConvertToList(), right);
        }

        if (right->GetItemType() == EItemType::Value)
        {
            ValueItemPtr rightValue{ ToValue(right) };
            return Compare(left, rightValue->ConvertToList());
        }

        checkNoEntry();
        return 0;
    }

    class Packet
    {
    public:
        Packet(const std::string& input)
            : contents{ input }
        {

        }

        int32 CompareTo(const Packet& right) const
        {
            for (size_t leftIndex{ 0 }, rightIndex{ 0 }; leftIndex < contents.items.size() && rightIndex < right.contents.items.size(); ++leftIndex, ++rightIndex)
            {
                ItemPtr leftItem{ contents.items[leftIndex] };
                ItemPtr rightItem{ right.contents.items[rightIndex] };
                int32 comparison{ Compare(leftItem, rightItem) };
                if (comparison != 0)
                {
                    return comparison;
                }
            }

            if (contents.items.size() < right.contents.items.size()) { return -1; }
            if (contents.items.size() == right.contents.items.size()) { return 0; }
            return 1;
        }

        auto operator<=>(const Packet& other) const
        {
            int32 comparison{ CompareTo(other) };
            if (comparison < 0) { return std::strong_ordering::less; }
            if (comparison == 0) { return std::strong_ordering::equal; }
            return std::strong_ordering::greater;
        }

        ListItem contents;
    };
}

void Puzzle13::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle13Helpers;

    check(input.size() % 3 == 0);
    int32 packetIndex{ 1 };
    int32 runningSum{ 0 };
    for (size_t index{ 0 }; index < input.size(); index += 3, ++packetIndex)
    {
        Packet left{ input[index] };
        Packet right{ input[index + 1] };

        if (left.CompareTo(right) < 0)
        {
            runningSum += packetIndex;
        }
    }

    std::cout << runningSum << std::endl;
}

void Puzzle13::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle13Helpers;

    check(input.size() % 3 == 0);
    std::vector<Packet> packets{ };
    packets.emplace_back(Packet{ "[[2]]" });
    packets.emplace_back(Packet{ "[[6]]" });
    for (size_t index{ 0 }; index < input.size(); index += 3)
    {
        Packet left{ input[index] };
        Packet right{ input[index + 1] };
        packets.emplace_back(left);
        packets.emplace_back(right);
    }

    std::sort(packets.begin(), packets.end());
    
    size_t startIndex{ 0 };
    size_t endIndex{ 0 };
    for (size_t index{ 0 }; index < packets.size(); ++index)
    {
        if (packets[index].contents.GetRaw() == "[[2]]") { startIndex = index + 1; }
        if (packets[index].contents.GetRaw() == "[[6]]") { endIndex = index + 1; break; }
    }

    std::cout << startIndex * endIndex << std::endl;
}
