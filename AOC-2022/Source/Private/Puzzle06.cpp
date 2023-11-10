#include "pch.h"
#include "Puzzles.h"
#include "Core.h"

namespace Puzzle06Helpers
{
    int32 GetIndexOfStartMessage(const std::string& sequence, int32 headerSize)
    {
        for (int32 index{ 0 }; index < sequence.size() - headerSize; ++index)
        {
            const std::string& header{ sequence.substr(index, headerSize) };
            std::set<char> uniqueCharacters{ };
            for (const auto& character : header)
            {
                uniqueCharacters.emplace(character);
            }
            if (uniqueCharacters.size() == headerSize)
            {
                return index + headerSize;
            }
        }
        checkNoEntry();
        return -1;
    }
}

void Puzzle06::SolveA(const std::vector<std::string>& input) const
{
    using namespace Puzzle06Helpers;

    check(input.size() == 1);
    const std::string& signal{ input[0] };

    std::cout << GetIndexOfStartMessage(signal, 4) << std::endl;
}

void Puzzle06::SolveB(const std::vector<std::string>& input) const
{
    using namespace Puzzle06Helpers;
    
    check(input.size() == 1);
    const std::string& signal{ input[0] };

    std::cout << GetIndexOfStartMessage(signal, 14) << std::endl;
}
