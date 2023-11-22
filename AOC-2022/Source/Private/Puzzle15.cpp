#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

using HorizontalCovering = Core::Math::Range<int64>;
IMPL_HASH(HorizontalCovering, value.GetLowerBound(), value.GetUpperBound());

namespace Puzzle15Helpers
{
    struct SensorCovering
    {
        Vector2d64 nearestBeaconCoordinate{ 0, 0 };
        int64 manhattanDistance{ 0 };
    };

    int64 CalculateManhattanDistance(const Vector2d64& lhs, const Vector2d64& rhs)
    {
        return std::abs(lhs.x - rhs.x) + std::abs(lhs.y - rhs.y);
    }

    std::unordered_map<Vector2d64, SensorCovering> CreateSensorBeaconMapping(const std::vector<std::string>& input)
    {
        std::unordered_map<Vector2d64, SensorCovering> sensorToBeaconMapping{ };
        sensorToBeaconMapping.reserve(input.size());

        for (const auto& line : input)
        {
            std::vector<std::string> sensorsAndBeacons{ Core::SplitString(line, ":") };
            check(sensorsAndBeacons.size() == 2);
            std::string sensorString{ sensorsAndBeacons[0].substr(10) };
            std::vector<std::string> sensorCoordinateString{ Core::SplitString(sensorString, ",") };
            check(sensorCoordinateString.size() == 2);
            Vector2d64 sensorCoordinate{ std::stoll(sensorCoordinateString[0].substr(2)), std::stoll(sensorCoordinateString[1].substr(3)) };
            std::string beaconString{ sensorsAndBeacons[1].substr(22) };
            std::vector<std::string> beaconCoordinateString{ Core::SplitString(beaconString, ",") };
            check(beaconCoordinateString.size() == 2);
            Vector2d64 beaconCoordinate{ std::stoll(beaconCoordinateString[0].substr(2)), std::stoll(beaconCoordinateString[1].substr(3)) };
            sensorToBeaconMapping.emplace(sensorCoordinate, SensorCovering{
                .nearestBeaconCoordinate{ beaconCoordinate },
                .manhattanDistance{ CalculateManhattanDistance(sensorCoordinate, beaconCoordinate) },
            });
        }

        return sensorToBeaconMapping;
    }
}

void Puzzle15::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle15Helpers;

    auto sensorToBeaconMapping{ CreateSensorBeaconMapping(input) };

    std::vector<HorizontalCovering> invalidRanges{ };

    const auto targetY{ 2000000ULL };
    for (const auto& [sensor, sensorCovering] : sensorToBeaconMapping)
    {
        const auto distanceToTargetLine{ CalculateManhattanDistance(sensor, Vector2d64{ sensor.x, targetY }) };
        if (sensorCovering.manhattanDistance <= distanceToTargetLine) { continue; }
        const auto overhead{ std::abs(distanceToTargetLine - sensorCovering.manhattanDistance) };
        invalidRanges.emplace_back(HorizontalCovering{ sensor.x - overhead, sensor.x + overhead });
    }

    std::sort(invalidRanges.begin(), invalidRanges.end());
    std::vector<HorizontalCovering> collapsedRanges{ };
    for (auto index{ 0 }; index < invalidRanges.size(); ++index)
    {
        HorizontalCovering lowestCovering{ invalidRanges[index] };
        while (index < invalidRanges.size() - 1 && lowestCovering.Overlaps(invalidRanges[index + 1]))
        {
            lowestCovering = lowestCovering.Merge(invalidRanges[index + 1]);
            ++index;
        }
        collapsedRanges.emplace_back(lowestCovering);
    }

    auto invalidLocations{ 0 };
    for (const auto& range : collapsedRanges)
    {
        invalidLocations += range.GetUpperBound() - range.GetLowerBound();
    }

    std::cout << invalidLocations << std::endl;
}

void Puzzle15::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle15Helpers;

    auto sensorToBeaconMapping{ CreateSensorBeaconMapping(input) };

    for (auto height{ 0 }; height <= 4000000; ++height)
    {
        std::vector<HorizontalCovering> invalidRanges{ };

        for (const auto& [sensor, sensorCovering] : sensorToBeaconMapping)
        {
            const auto distanceToTargetLine{ CalculateManhattanDistance(sensor, Vector2d64{ sensor.x, height }) };
            if (sensorCovering.manhattanDistance <= distanceToTargetLine) { continue; }
            const auto overhead{ std::abs(distanceToTargetLine - sensorCovering.manhattanDistance) };
            invalidRanges.emplace_back(HorizontalCovering{ std::max(sensor.x - overhead, 0LL), std::min(sensor.x + overhead, 4000000LL) });
        }

        std::sort(invalidRanges.begin(), invalidRanges.end());
        std::vector<HorizontalCovering> collapsedRanges{ };
        for (auto index{ 0 }; index < invalidRanges.size(); ++index)
        {
            HorizontalCovering lowestCovering{ invalidRanges[index] };
            while (index < invalidRanges.size() - 1 && lowestCovering.Overlaps(invalidRanges[index + 1]))
            {
                lowestCovering = lowestCovering.Merge(invalidRanges[index + 1]);
                ++index;
            }
            collapsedRanges.emplace_back(lowestCovering);
        }

        if (collapsedRanges.size() > 1)
        {
            check(collapsedRanges.size() == 2);
            std::cout << (collapsedRanges[0].GetUpperBound() + 1) * 4000000 + height << std::endl;
            break;
        }
    }
}
