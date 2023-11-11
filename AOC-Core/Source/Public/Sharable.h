#pragma once

#include "pch.h"

#define DECL_SHARABLE(type) \
    class type; \
    using type##Ptr = std::shared_ptr<type>; \
    using type##ConstPtr = std::shared_ptr<const type>; \

#define IMPL_SHARABLE(type) \
    public: \
    type##Ptr ToShared() { return shared_from_this(); } \
    type##ConstPtr ToShared() const { return shared_from_this(); } \
    private: \
