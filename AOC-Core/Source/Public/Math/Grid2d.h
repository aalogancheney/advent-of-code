#pragma once

#include "pch.h"

namespace Core::Math
{
    template<typename T>
    class Grid2d
    {
    public:
        Grid2d(size_t inWidth, size_t inHeight)
            : width{ inWidth }
            , height{ inHeight }
            , elements(width * height)
        {

        }

        size_t GetWidth() const { return width; }
        size_t GetHeight() const { return height; }

        const T& at(size_t x, size_t y) const
        {
            check(x < GetWidth());
            check(y < GetHeight());
            return elements.at(y * GetWidth() + x);
        }

        T& at(size_t x, size_t y)
        {
            check(x < GetWidth());
            check(y < GetHeight());
            return elements.at(y * GetWidth() + x);
        }

        void Print() const
        {
            for (auto rowIndex{ 0 }; rowIndex < GetHeight(); ++rowIndex)
            {
                for (auto colIndex{ 0 }; colIndex < GetWidth(); ++colIndex)
                {
                    std::cout << at(colIndex, rowIndex);
                }
                std::cout << std::endl;
            }
        }

    private:
        size_t width{ 0 };
        size_t height{ 0 };
        std::vector<T> elements{ };
    };
}
