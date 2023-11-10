#pragma once

#include "pch.h"
#include "Core.h"

namespace Puzzle10Helpers
{
#define DECL_INSTRUCTION() \
    public: \
        static const std::string& instructionName; \
        virtual int32 GetCycles() const override; \
    private: \

#define IMPL_INSTRUCTION(t, n, c) \
    const std::string& t::instructionName{ n }; \
    int32 t::GetCycles() const { return c; } \

    class CPU;

    class IInstruction
    {
    public:
        virtual int32 GetCycles() const { return -1; }

        void Load(CPU& cpu);
        bool Execute(CPU& cpu);
        virtual void Store(CPU& cpu) { }

    private:
        int32 startCycles{ -1 };
    };

    class NoOpCommand : public IInstruction
    {
        DECL_INSTRUCTION();
    };

    class AddX : public IInstruction
    {
        DECL_INSTRUCTION();

    public:
        AddX(int32 inValue) : value{ inValue } { }
        virtual void Store(CPU& cpu) override;

    private:
        int32 value;
    };
}
