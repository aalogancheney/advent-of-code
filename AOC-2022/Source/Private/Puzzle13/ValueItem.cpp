#include "pch.h"

#include "Core.h"
#include "ValueItem.h"
#include "ListItem.h"

ValueItem::ValueItem(const std::string& input)
    : Item{ input }
{
    check(input.size() >= 0);
    check(input[0] != '[');

    value = std::stoi(input);
}

ItemPtr ValueItem::ConvertToList() const
{
    return std::make_shared<ListItem>(ListItem{ "[" + GetRaw() + "]" });
}
