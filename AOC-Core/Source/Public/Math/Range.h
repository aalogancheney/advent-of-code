#pragma once

#include "pch.h"

#include "Concepts.h"

namespace Core
{
    namespace Math
    {
        template<Arithmetic T>
        struct Range
        {
            Range(T inLowerBound, T inUpperBound)
                : lowerBound{ inLowerBound }
                , upperBound{ inUpperBound }
            {

            }

            bool ContainsInclusive(const T& value) const 
            {
                return value >= lowerBound && value <= upperBound;
            }

            bool ContainsInclusive(const Range& other) const
            {
                return lowerBound <= other.lowerBound && upperBound >= other.upperBound;
            }

            bool ContainsExclusive(const T& value) const
            {
                return value > lowerBound && value < upperBound;
            }

            bool ContainsExclusive(const Range& other) const
            {
                return lowerBound < other.lowerBound && upperBound > other.upperBound;
            }

            bool IsContainedWithinInclusive(const Range& other) const
            {
                return other.ContainsInclusive(*this);
            }

            bool IsContainedWithinExclusive(const Range& other) const
            {
                return other.ContainsExclusive(*this);
            }

            bool OverlapsLower(const Range& other) const
            {
                return lowerBound <= other.lowerBound && upperBound >= other.lowerBound && upperBound <= other.upperBound;
            }

            bool OverlapsUpper(const Range& other) const
            {
                return other.OverlapsLower(*this);
            }

            bool Intersects(const Range& other) const
            {
                return upperBound >= other.lowerBound || lowerBound <= other.upperBound;
            }

            bool Overlaps(const Range& other) const
            {
                return OverlapsLower(other) || OverlapsUpper(other);
            }

            Range Merge(const Range& other)
            {
                if (!Overlaps(other))
                {
                    return Range{ lowerBound, upperBound };
                }
                return Range{ std::min(lowerBound, other.lowerBound), std::max(upperBound, other.upperBound) };
            }

            auto operator<=>(const Range&) const = default;
            bool operator==(const Range&) const = default;

            constexpr const T& GetLowerBound() const { return lowerBound; }
            constexpr const T& GetUpperBound() const { return upperBound; }
            
            void SetUpperBound(const T& value)
            {
                check(value >= lowerBound);
                upperBound = std::max(value, lowerBound);
            }

            void SetLowerBound(const T& value)
            {
                check(value <= upperBound);
                lowerBound = std::min(value, upperBound);
            }

            void SetBounds(const T& lower, const T& upper)
            {
                check(lower <= upper);
                lowerBound = lower;
                upperBound = upper;
            }

        private:
            T lowerBound{ };
            T upperBound{ };
        };
    }
}
