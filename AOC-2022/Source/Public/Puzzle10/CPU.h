#pragma once

#include "pch.h"
#include "Core.h"

namespace Puzzle10Helpers
{
    class Program;

    class CPU
    {
    public:
        void AdvanceCycles();
        void ExecuteProgram(Program& program);

        int32 CalculateOutput() const;
        void PrintCRTOutput() const;

        int32 GetCycles() { return cycles; }
        int32& GetX() { return x; }

    private:
        int32 x{ 1 };
        int32 cycles{ 0 };

        std::vector<int32> savedRegisterValues{ };
        std::vector<char> crtPixels{ };

        const int32 crtWidth{ 40 };
    };
}
