#pragma once

#include "pch.h"

namespace Core
{
	// Creates a new hash by hashing the provided values. Algorithm taken from Boost's hash_combine function.
	// https://www.boost.org/doc/libs/1_37_0/doc/html/hash/reference.html#boost.hash_combine
	template<typename T>
	void HashCombine(size_t& seed, const T& value)
	{
		auto hasher = std::hash<T>{};
		seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	// Variadic hash combiner.
	template<typename T, typename... Args>
	void HashCombine(size_t& seed, const T& value, const Args&... args)
	{
		HashCombine(seed, value);
		HashCombine(seed, args...);
	}
}

#define IMPL_HASH(T, ...) \
	namespace std \
	{ \
		template<> \
		struct hash<T> \
		{ \
			size_t operator()(const T& value) const \
			{ \
				size_t result{ 0ull }; \
				Core::HashCombine(result, __VA_ARGS__); \
				return result; \
			} \
		}; \
	} \
