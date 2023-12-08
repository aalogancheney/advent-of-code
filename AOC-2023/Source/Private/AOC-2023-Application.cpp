#include "pch.h"

#include "AOC-2023-Application.h"
#include "Puzzles.h"

Core::Application* Core::CreateApplication()
{
    return new AOC2023Application();
}

void AOC2023Application::Run()
{
    auto puzzle{ Puzzle08{ } };
    puzzle.Solve();
}
