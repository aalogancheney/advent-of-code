#pragma once

#include "pch.h"

namespace Core
{
	std::vector<std::string> CORE_API ReadAllLinesInFile(const std::filesystem::path& path);

	std::vector<std::string> CORE_API SplitString(const std::string& input, const std::string& delimiter);
}
