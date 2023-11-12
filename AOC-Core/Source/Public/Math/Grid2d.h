#pragma once

#include "pch.h"

#include "Vector2d.h"

namespace Core::Math
{
    using Grid2dCoordinate = Vector2D<size_t>;

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

        const T& at(Grid2dCoordinate coordinate) const
        {
            return at(coordinate.x, coordinate.y);
        }

        T& at(Grid2dCoordinate coordinate)
        {
            return at(coordinate.x, coordinate.y);
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
