#include "pch.h"

#include "Puzzle.h"
#include "Input.h"
#include "ScopedTimer.h"

namespace Core
{
	Puzzle::Puzzle(const std::filesystem::path& inputFile)
		: input{ Core::ReadAllLinesInFile(inputFile) }
	{

	}

	void Puzzle::Solve() const
	{
		{
			const auto partATimer{ ScopedTimer("Part A") };
			SolveA(input);
		}
		
		{
			const auto partATimer{ ScopedTimer("Part B") };
			SolveB(input);
		}
	}
}
