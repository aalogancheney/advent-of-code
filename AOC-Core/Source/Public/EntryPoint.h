#pragma once

#include "pch.h"
#include "Application.h"

extern Core::Application* Core::CreateApplication();

int main(int argc, char** argv)
{
	auto Application{ Core::CreateApplication() };
	Application->Run();
	delete Application;
}
