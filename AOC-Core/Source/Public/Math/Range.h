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
                    || (upperBound <= other.upperBound && upperBound >= other.lowerBound);
            }

        private:
            T lowerBound{ };
            T upperBound{ };
        };
    }
}
