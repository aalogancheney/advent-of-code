#include "pch.h"

#include "AOC-2022-Application.h"
#include "Puzzles.h"

Core::Application* Core::CreateApplication()
{
	return new AOC2022Application();
}

void AOC2022Application::Run()
{
	auto puzzle{ Puzzle05{} };
	puzzle.Solve();
}
