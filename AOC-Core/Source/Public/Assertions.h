#pragma once

#include "pch.h"

#if AOC_DEBUG

#define check(condition) \
	std::assert(condition); \
	if (!(condition)) { \
		std::terminate(); \
	} \

#define checkNoEntry() \
	check(false); \

#else

#define check(condition)
#define checkNoEntry()

#endif
