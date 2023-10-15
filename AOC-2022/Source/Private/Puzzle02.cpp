#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle02Helpers
{
	enum class EMove { Rock, Paper, Scissors };
	static constexpr int32 moveScores[]{ 1, 2, 3 };

	enum class EOutcome { Lost, Draw, Win };
	static constexpr int32 outcomeScores[]{ 0, 3, 6 };

	EMove TranslateOpponentMove(char input)
	{
		check(input == 'A' || input == 'B' || input == 'C');
		static constexpr EMove moves[]{ EMove::Rock, EMove::Paper, EMove::Scissors };
		return moves[static_cast<int32>(input - 'A')];
	}

	EMove TranslateMyMoveA(char input)
	{
		check(input == 'X' || input == 'Y' || input == 'Z');
		static constexpr EMove moves[] { EMove::Rock, EMove::Paper, EMove::Scissors };
		return moves[static_cast<int32>(input - 'X')];
	}

	EMove TranslateMyMoveB(EMove opponentMove, char input)
	{
		check(input == 'X' || input == 'Y' || input == 'Z');
		static constexpr EOutcome outcomes[] { EOutcome::Lost, EOutcome::Draw, EOutcome::Win };
		EOutcome desiredOutcome{ outcomes[static_cast<int32>(input - 'X')] };

		static constexpr EMove moves[3][3]
		{					  /*Rock*/			/*Paper*/			/*Scissors*/
			/*Lost*/		{ EMove::Scissors,	EMove::Rock,		EMove::Paper },
			/*Draw*/		{ EMove::Rock,		EMove::Paper,		EMove::Scissors },
			/*Win*/			{ EMove::Paper,		EMove::Scissors,	EMove::Rock },
		};

		return moves[static_cast<int32>(desiredOutcome)][static_cast<int32>(opponentMove)];
	}

	EOutcome GetOutcome(EMove opponentMove, EMove myMove)
	{
		static constexpr EOutcome outcomes[3][3]
		{					  /*Rock*/			/*Paper*/		/*Scissors*/
			/*Rock*/		{ EOutcome::Draw,	EOutcome::Win,	EOutcome::Lost },
			/*Paper*/		{ EOutcome::Lost,	EOutcome::Draw, EOutcome::Win },
			/*Scissors*/	{ EOutcome::Win,	EOutcome::Lost, EOutcome::Draw },
		};
		return outcomes[static_cast<int32>(opponentMove)][static_cast<int32>(myMove)];
	}
}

void Puzzle02::SolveA(const std::vector<std::string>& input) const
{
	using namespace Puzzle02Helpers;

	int32 score{ 0 };
	for (const auto& game : input)
	{
		check(game.size() == 3);
		EMove opponentMove{ TranslateOpponentMove(game[0]) };
		EMove myMove{ TranslateMyMoveA(game[2]) };
		EOutcome outcome{ GetOutcome(opponentMove, myMove) };
		score += outcomeScores[static_cast<int32>(outcome)];
		score += moveScores[static_cast<int32>(myMove)];
	}
	std::cout << "Score: " << score << std::endl;
}

void Puzzle02::SolveB(const std::vector<std::string>& input) const
{
	using namespace Puzzle02Helpers;

	int32 score{ 0 };
	for (const auto& game : input)
	{
		check(game.size() == 3);
		EMove opponentMove{ TranslateOpponentMove(game[0]) };
		EMove myMove{ TranslateMyMoveB(opponentMove, game[2]) };
		EOutcome outcome{ GetOutcome(opponentMove, myMove) };
		score += outcomeScores[static_cast<int32>(outcome)];
		score += moveScores[static_cast<int32>(myMove)];
	}
	std::cout << "Score: " << score << std::endl;
}
