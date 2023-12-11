#pragma once

#include "pch.h"

#include "Vector2.h"
#include "Concepts.h"

namespace Core::Math
{
    template<typename TElement, SignedIntegral TSize>
    class Grid2d
    {
    public:
        using Grid2dCoordinate = Vector2<TSize>;
        using Grid2dElement = std::pair<Grid2dCoordinate, TElement>;
        
        Grid2d(TSize inWidth, TSize inHeight)
            : width{ inWidth }
            , height{ inHeight }
            , elements(static_cast<size_t>(width * height))
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

        auto IndexToCoordinate(TSize index) const
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

        auto IsInBounds(Grid2dCoordinate coordinate) const
        {
            return coordinate.x >= 0 && coordinate.x < GetWidth()
                && coordinate.y >= 0 && coordinate.y < GetHeight();
        }

        constexpr const Grid2dElement& at(TSize x, TSize y) const
        {
            return elements.at(CoordinateToIndex({ x, y }));
        }

        constexpr Grid2dElement& at(TSize x, TSize y)
        {
            return elements.at(CoordinateToIndex({ x, y }));
        }

        constexpr const Grid2dElement& at(Grid2dCoordinate coordinate) const
        {
            return elements.at(CoordinateToIndex(coordinate));
        }

        constexpr Grid2dElement& at(Grid2dCoordinate coordinate)
        {
            return elements.at(CoordinateToIndex(coordinate));
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
        TSize width{ 0 };
        TSize height{ 0 };
        std::vector<Grid2dElement> elements{ };
    };

    CORE_API Grid2d<char, int32> ConstructGrid(const std::vector<std::string>& input);
}
