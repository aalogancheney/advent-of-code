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
            std::cout << "Starting Part A..." << std::endl;
            const auto partATimer{ ScopedTimer("Part A") };
            SolveA(input);
        }
        
        std::cout << std::endl;

        {
            std::cout << "Starting Part B..." << std::endl;
            const auto partATimer{ ScopedTimer("Part B") };
            SolveB(input);
        }
    }
}
