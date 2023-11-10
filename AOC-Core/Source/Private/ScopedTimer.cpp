#include "pch.h"

#include "ScopedTimer.h"

namespace Core
{
    ScopedTimer::ScopedTimer(const std::string& message)
        : message{ message }
        , start{ std::chrono::high_resolution_clock::now() }
    {

    }

    ScopedTimer::~ScopedTimer()
    {
        const auto stop{ std::chrono::high_resolution_clock::now() };
        auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "Finished " << message << " in " << durationMs.count() << " ms." << std::endl;
    }
}
