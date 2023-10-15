#pragma once

#include "pch.h"

#include <vector>
#include <string>
#include <filesystem>

namespace Core
{
	std::vector<std::string> ReadAllLinesInFile(const std::filesystem::path& path);
}
