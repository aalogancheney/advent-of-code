#include "pch.h"
#include "Application.h"

namespace Core
{
    Application* CreateApplication()
    {
        return new Application();
    }

    void Application::Run()
    {
        while (true);
    }
}
