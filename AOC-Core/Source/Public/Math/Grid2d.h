#pragma once

#include "pch.h"

#include "Vector2.h"

DECL_Vector2(Grid2dCoordinate, size_t);

namespace Core::Math
{
    template<typename T>
    class Grid2d
    {
    public:
        using Grid2dElement = std::tuple<Grid2dCoordinate, T>;
        
        Grid2d(size_t inWidth, size_t inHeight)
            : width{ inWidth }
            , height{ inHeight }
            , elements(width * height)
        {
            for (auto index{ 0 }; index < elements.size(); ++index)
            {
                std::get<0>(elements[index]) = IndexToCoordinate(index);
            }
        }

        std::vector<Grid2dElement>::iterator begin() noexcept { return elements.begin(); }
        std::vector<Grid2dElement>::iterator end() noexcept { return elements.end(); }
        std::vector<Grid2dElement>::const_iterator begin() const noexcept { return elements.begin(); }
        std::vector<Grid2dElement>::const_iterator end() const noexcept { return elements.end(); }
        std::vector<Grid2dElement>::const_iterator cbegin() const noexcept { return elements.cbegin(); }
        std::vector<Grid2dElement>::const_iterator cend() const noexcept { return elements.cend(); }
        std::vector<Grid2dElement>::reverse_iterator rbegin() noexcept { return elements.rbegin(); }
        std::vector<Grid2dElement>::reverse_iterator rend() noexcept { return elements.rend(); }
        std::vector<Grid2dElement>::const_reverse_iterator rbegin() const noexcept { return elements.rbegin(); }
        std::vector<Grid2dElement>::const_reverse_iterator rend() const noexcept { return elements.rend(); }
        std::vector<Grid2dElement>::const_reverse_iterator crbegin() const noexcept { return elements.crbegin(); }
        std::vector<Grid2dElement>::const_reverse_iterator crend() const noexcept { return elements.crend(); }

        size_t GetWidth() const noexcept { return width; }
        size_t GetHeight() const noexcept { return height; }
        size_t size() const noexcept { return elements.size(); }

        Grid2dCoordinate IndexToCoordinate(size_t index) const
        {
            check(index < size());
            return Grid2dCoordinate{ index % width, index / width };
        }

        size_t CoordinateToIndex(Grid2dCoordinate coordinate) const
        {
            check(coordinate.x < width);
            check(coordinate.y < height);
            return coordinate.x + coordinate.y * height;
        }

        constexpr const T& at(size_t x, size_t y) const
        {
            return std::get<1>(elements.at(CoordinateToIndex(Grid2dCoordinate{ x, y })));
        }

        constexpr T& at(size_t x, size_t y)
        {
            return std::get<1>(elements.at(CoordinateToIndex(Grid2dCoordinate{ x, y })));
        }

        constexpr const T& at(Grid2dCoordinate coordinate) const
        {
            return std::get<1>(elements.at(CoordinateToIndex(coordinate)));
        }

        constexpr T& at(Grid2dCoordinate coordinate)
        {
            return std::get<1>(elements.at(CoordinateToIndex(coordinate)));
        }

        void Print() const
        {
            for (auto rowIndex{ 0 }; rowIndex < GetHeight(); ++rowIndex)
            {
                for (auto colIndex{ 0 }; colIndex < GetWidth(); ++colIndex)
                {
                    std::cout << at(colIndex, rowIndex);
                }
                std::cout << "\n";
            }
        }

    private:
        size_t width{ 0 };
        size_t height{ 0 };
        std::vector<Grid2dElement> elements{ };
    };
}
