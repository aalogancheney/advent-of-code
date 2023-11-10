#include "pch.h"
#include "Input.h"

namespace Core
{
    std::vector<std::string> ReadAllLinesInFile(const std::filesystem::path& path)
    {
        auto lines{ std::vector<std::string>{ } };
        if (std::filesystem::exists(path))
        {
            auto FileStream{ std::ifstream{ path } };
            auto Line{ std::string{ "" } };
            while (std::getline(FileStream, Line))
            {
                lines.emplace_back(Line);
            }
        }
        return lines;
    }

    std::vector<std::string> SplitString(const std::string& input, const std::string& delimiter)
    {
        std::vector<std::string> splitString{ };

        size_t last{ 0 };
        size_t next{ 0 };
        while ((next = input.find(delimiter, last)) != std::string::npos)
        {
            splitString.emplace_back(input.substr(last, next - last));
            last = next + 1;
        }
        splitString.emplace_back(input.substr(last));
        return splitString;
    }
}
