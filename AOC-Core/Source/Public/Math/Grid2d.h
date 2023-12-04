#pragma once

#include "pch.h"

#include "Vector2.h"
#include "Concepts.h"

namespace Core::Math
{
    template<typename TElement, SignedIntegral TCoordinate>
    class Grid2d
    {
    public:
        using Grid2dCoordinate = Vector2<TCoordinate>;
        using Grid2dElement = std::tuple<Grid2dCoordinate, TElement>;
        
        Grid2d(TCoordinate inWidth, TCoordinate inHeight)
            : width{ inWidth }
            , height{ inHeight }
            , elements(width * height)
        {
            for (auto index{ 0 }; index < elements.size(); ++index)
            {
                std::get<0>(elements[index]) = IndexToCoordinate(index);
            }
        }

        auto begin() noexcept { return elements.begin(); }
        auto end() noexcept { return elements.end(); }
        auto begin() const noexcept { return elements.begin(); }
        auto end() const noexcept { return elements.end(); }
        auto cbegin() const noexcept { return elements.cbegin(); }
        auto cend() const noexcept { return elements.cend(); }
        auto rbegin() noexcept { return elements.rbegin(); }
        auto rend() noexcept { return elements.rend(); }
        auto rbegin() const noexcept { return elements.rbegin(); }
        auto rend() const noexcept { return elements.rend(); }
        auto crbegin() const noexcept { return elements.crbegin(); }
        auto crend() const noexcept { return elements.crend(); }

        auto GetWidth() const noexcept { return width; }
        auto GetHeight() const noexcept { return height; }
        auto size() const noexcept { return elements.size(); }

        auto IndexToCoordinate(TCoordinate index) const
        {
            check(index < size());
            return Grid2dCoordinate{ index % width, index / width };
        }

        auto CoordinateToIndex(Grid2dCoordinate coordinate) const
        {
            check(coordinate.x < width);
            check(coordinate.y < height);
            return coordinate.x + coordinate.y * height;
        }

        constexpr const TElement& at(TCoordinate x, TCoordinate y) const
        {
            return std::get<1>(elements.at(CoordinateToIndex({ x, y })));
        }

        constexpr TElement& at(TCoordinate x, TCoordinate y)
        {
            return std::get<1>(elements.at(CoordinateToIndex({ x, y })));
        }

        constexpr const TElement& at(Grid2dCoordinate coordinate) const
        {
            return std::get<1>(elements.at(CoordinateToIndex(coordinate)));
        }

        constexpr TElement& at(Grid2dCoordinate coordinate)
        {
            return std::get<1>(elements.at(CoordinateToIndex(coordinate)));
        }

        auto Print() const
        {
            for (const auto& [coordinate, value] : elements)
            {
                if (coordinate.x == 0)
                {
                    std::cout << "\n";
                }
                std::cout << value;
            }
        }

    private:
        TCoordinate width{ 0 };
        TCoordinate height{ 0 };
        std::vector<Grid2dElement> elements{ };
    };

    CORE_API Grid2d<char, int32> ConstructGrid(const std::vector<std::string>& input);
}
