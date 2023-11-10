#include "pch.h"

#include "Core.h"
#include "ListItem.h"
#include "ValueItem.h"

ListItem::ListItem(const std::string& input)
    : Item{ input }
{
    check(input.size() >= 2);
    check(input[0] == '[');
    check(input[input.size() - 1] == ']');

    // Strip the start and end brackets to get the contents.
    std::string contents{ input.substr(1, input.size() - 2) };

    // Iterate over the contents. Assume all lists are well-formatted. Incrementing index at the end of the loop
    // will always consume the comma separating items.
    for (size_t index{ 0 }; index < contents.size(); ++index)
    {
        // If we encounter a start bracket, search for the matching closing bracket and parse that list recursively.
        if (contents[index] == '[')
        {
            size_t start{ index };
            size_t depth{ 1 };
            ++index;
            while (depth != 0)
            {
                if (contents[index] == '[') { ++depth; }
                if (contents[index] == ']') { --depth; }
                ++index;
            }
            std::string list{ contents.substr(start, index - start) };
            items.emplace_back(new ListItem{ list });
        }
        else
        {
            // Otherwise, we must be on a value. Scan for the end of the value (either the end of the string or
            // a comma), then parse that item.
            size_t start{ index };
            while (index < contents.size() && contents[index] != ',')
            {
                ++index;
            }
            std::string value{ contents.substr(start, index - start) };
            items.emplace_back(new ValueItem{ value });
        }
    }
}
