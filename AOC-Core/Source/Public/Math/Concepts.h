#pragma once

#include "pch.h"

template<typename T>
concept SignedArithmetic = (std::is_integral_v<T> || std::is_floating_point_v<T>);

template<typename T>
concept SignedIntegral = std::is_integral_v<T> && std::is_signed_v<T>;
