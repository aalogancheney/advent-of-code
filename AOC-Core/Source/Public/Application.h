#pragma once

#include "pch.h"
#include "CoreMacros.h"

namespace Core
{
	class CORE_API Application
	{
	public:
		Application() = default;
		virtual ~Application() = default;

	public:
		virtual void Run();
	};

	Application* CreateApplication();
}