#pragma once

#include "pch.h"

#ifdef AOC_CORE_BUILD_DLL
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif
