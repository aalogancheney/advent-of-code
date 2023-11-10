#include "pch.h"

#include "Puzzle10/Instructions.h"
#include "Puzzle10/CPU.h"

namespace Puzzle10Helpers
{
    void IInstruction::Load(CPU& cpu)
    {
        startCycles = cpu.GetCycles();
    }

    bool IInstruction::Execute(CPU& cpu)
    {
        return startCycles + GetCycles() == cpu.GetCycles();
    }

    IMPL_INSTRUCTION(NoOpCommand, "noop", 0);
    IMPL_INSTRUCTION(AddX, "addx", 1);

    void AddX::Store(CPU& cpu)
    {
        cpu.GetX() += value;
    }
}
