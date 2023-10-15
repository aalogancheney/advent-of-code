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
}
