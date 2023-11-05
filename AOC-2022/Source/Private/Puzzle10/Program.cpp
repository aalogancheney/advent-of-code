#include "pch.h"
#include "Core.h"
#include "Puzzle10/Program.h"
#include "Puzzle10/Instructions.h"

namespace Puzzle10Helpers
{
	Program::Program(const std::vector<std::string>& input)
	{
		for (const auto& instruction : input)
		{
			auto splitInstruction{ Core::SplitString(instruction, " ") };
			check(splitInstruction.size() > 0);
			if (splitInstruction.at(0) == NoOpCommand::instructionName)
			{
				check(splitInstruction.size() == 1);
				instructions.emplace_back(new NoOpCommand{ });
			}
			else if (splitInstruction.at(0) == AddX::instructionName)
			{
				check(splitInstruction.size() == 2);
				int32 value{ std::stoi(splitInstruction.at(1)) };
				instructions.emplace_back(new AddX{ value });
			}
		}
	}

	Program::~Program()
	{
		for (auto& instruction : instructions)
		{
			delete instruction;
		}
	}
}
