#include "pch.h"
#include "Puzzle10/CPU.h"
#include "Puzzle10/Program.h"
#include "Puzzle10/Instructions.h"

namespace Puzzle10Helpers
{
    void CPU::ExecuteProgram(Program& program)
    {
        program.Reset();
        while (!program.IsComplete())
        {
            AdvanceCycles();
            IInstruction* instruction{ program.GetCurrentInstruction() };
            instruction->Load(*this);
            while (!instruction->Execute(*this))
            {
                AdvanceCycles();
            }
            instruction->Store(*this);

            program.AdvanceInstruction();
        }
    }

    void CPU::AdvanceCycles()
    {
        ++cycles;

        if (cycles == 20 || ((cycles - 20) % 40) == 0)
        {
            savedRegisterValues.emplace_back(x * cycles);
        }

        const int32 crtPosition{ (cycles - 1) % crtWidth };
        if (std::abs(x - crtPosition) <= 1)
        {
            crtPixels.emplace_back('#');
        }
        else
        {
            crtPixels.emplace_back('.');
        }
    }

    int32 CPU::CalculateOutput() const
    {
        int32 sum{ 0 };
        for (const auto& savedValue : savedRegisterValues)
        {
            sum += savedValue;
        }
        return sum;
    }

    void CPU::PrintCRTOutput() const
    {
        for (int32 index{ 0 }; index < crtPixels.size(); ++index)
        {
            std::cout << crtPixels[index];
            if (((index + 1) % crtWidth) == 0)
            {
                std::cout << "\n";
            }
        }
    }
}
