#pragma once

#include "pch.h"

namespace Core
{
    namespace Math
    {
        template<typename T>
        class Range
        {
        public:
            Range(T inLowerBound, T inUpperBound)
                : lowerBound{ inLowerBound }
                , upperBound{ inUpperBound }
            {

            }

            bool IsContainedBy(const Range& other)
            {
                return lowerBound >= other.lowerBound
                    && upperBound <= other.upperBound;
            }

            bool Overlaps(const Range& other)
            {
                return (lowerBound >= other.lowerBound && lowerBound <= other.upperBound)
                    || (upperBound <= other.upperBound && upperBound >= other.lowerBound)
                    || (other.lowerBound >= lowerBound && other.lowerBound <= upperBound)
                    || (other.upperBound <= upperBound && other.upperBound >= lowerBound);
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

            const T& GetLowerBound() const { return lowerBound; }
            const T& GetUpperBound() const { return upperBound; }

        private:
            T lowerBound{ };
            T upperBound{ };
        };
    }
}
