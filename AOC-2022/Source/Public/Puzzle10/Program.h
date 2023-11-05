#pragma once

#include "pch.h"
#include "Core.h"

#include "Puzzle10/Instructions.h"

namespace Puzzle10Helpers
{
	class IInstruction;

	class Program
	{
	public:
		Program(const std::vector<std::string>& input);
		~Program();

		IInstruction* GetCurrentInstruction() { return instructions.at(currentInstruction); }
		void AdvanceInstruction() { ++currentInstruction; }
		bool IsComplete() const { return currentInstruction >= instructions.size(); }
		void Reset() { currentInstruction = 0; }

	private:
		int32 currentInstruction{ 0 };
		std::vector<IInstruction*> instructions{ };
	};
}