#pragma once

#include "pch.h"

#include "Item.h"

class ListItem : public Item
{
public:
    ListItem(const std::string& input);

    virtual EItemType GetItemType() const { return EItemType::List; }

    std::vector<ItemPtr> items{ };
};

using ListItemPtr = std::shared_ptr<ListItem>;
