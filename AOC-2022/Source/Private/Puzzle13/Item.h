#pragma once

#include "pch.h"

enum class EItemType
{
    Value,
    List,
};

class Item
{
public:
    Item(const std::string& inRaw)
        : raw{ inRaw }
    {

    }

    virtual EItemType GetItemType() const = 0;

    const std::string& GetRaw() const { return raw; }

private:
    std::string raw{ "" };
};

using ItemPtr = std::shared_ptr<Item>;
