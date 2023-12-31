#pragma once

#include "Core.h"
#include "Application.h"
#include "EntryPoint.h"

class AOC2023Application : public Core::Application
{
public:
    virtual void Run() override;
};

Core::Application* Core::CreateApplication();
