#pragma once

#include "pch.h"

namespace Core
{
	class Puzzle
	{
	public:
		CORE_API Puzzle(const std::filesystem::path& inputFile);

		void CORE_API Solve() const;

	protected:
		virtual void SolveA(const std::vector<std::string>& input) const = 0;
		virtual void SolveB(const std::vector<std::string>& input) const = 0;

	private:
		std::vector<std::string> input{ };
	};
}

#define STRING(input) #input

#define DECLARE_PUZZLE(number) \
	class Puzzle##number final : public Core::Puzzle \
	{ \
	public: \
		Puzzle##number() \
			: Puzzle(STRING(Input/Puzzle##number.in)) \
		{ } \
	protected: \
		virtual void SolveA(const std::vector<std::string>& input) const override; \
		virtual void SolveB(const std::vector<std::string>& input) const override; \
	}; \
