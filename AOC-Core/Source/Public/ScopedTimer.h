#pragma once

#include "pch.h"

namespace Core
{
    class ScopedTimer
    {
    public:
        ScopedTimer(const std::string& message = "");
        ~ScopedTimer();

    private:
        std::string message{ };
        std::chrono::high_resolution_clock::time_point start{ };
    };
}
