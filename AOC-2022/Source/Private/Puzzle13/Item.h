#pragma once

#include "pch.h"

#include "Core.h"

enum class EItemType
{
    Value,
    List,
};

DECL_SHARABLE(Item);

class Item : public std::enable_shared_from_this<Item>
{
    IMPL_SHARABLE(Item);

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
