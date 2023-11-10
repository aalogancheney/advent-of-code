#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

#include "Puzzle10/CPU.h"
#include "Puzzle10/Program.h"

void Puzzle10::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle10Helpers;

    Program program{ input };
    CPU cpu{ };
    cpu.ExecuteProgram(program);
    std::cout << cpu.CalculateOutput() << std::endl;
}

void Puzzle10::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle10Helpers;

    Program program{ input };
    CPU cpu{ };
    cpu.ExecuteProgram(program);
    cpu.PrintCRTOutput();
}
