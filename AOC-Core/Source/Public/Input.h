#pragma once

#include "pch.h"

namespace Core
{
    CORE_API std::vector<std::string> ReadAllLinesInFile(const std::filesystem::path& path);

    CORE_API std::vector<std::string> SplitString(const std::string& input, const std::string& delimiter);
}
