#pragma once

#include "pch.h"

#include "Item.h"

class ValueItem : public Item
{
public:
    ValueItem(const std::string& input);

    virtual EItemType GetItemType() const { return EItemType::Value; }

    ItemPtr ConvertToList() const;

    int32 value{ 0 };
};

using ValueItemPtr = std::shared_ptr<ValueItem>;
