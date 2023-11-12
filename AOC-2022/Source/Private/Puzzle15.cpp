#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle15Helpers
{
    std::unordered_map<Vector2d64, Vector2d64> CreateSensorBeaconMapping(const std::vector<std::string>& input)
    {
        std::unordered_map<Vector2d64, Vector2d64> sensorToBeaconMapping{ };
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
            sensorToBeaconMapping.emplace(sensorCoordinate, beaconCoordinate);
        }

        return sensorToBeaconMapping;
    }
}

void Puzzle15::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle15Helpers;

    auto sensorToBeaconMapping{ CreateSensorBeaconMapping(input) };
}

void Puzzle15::SolveB(const std::vector<std::string>& input) const
{
    
}
