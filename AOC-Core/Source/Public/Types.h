#pragma once

#include "pch.h"

using uint8 = unsigned char;
using uint16 = unsigned short int;
using uint32 = unsigned int;
using uint64 = unsigned long long;

using int8 = signed char;
using int16 = signed short int;
using int32 = signed int;
using int64 = signed long long;

static_assert(sizeof(uint8)    == 1);
static_assert(sizeof(uint16) == 2);
static_assert(sizeof(uint32) == 4);
static_assert(sizeof(uint64) == 8);

static_assert(sizeof(int8) == 1);
static_assert(sizeof(int16) == 2);
static_assert(sizeof(int32) == 4);
static_assert(sizeof(int64) == 8);
