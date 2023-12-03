workspace "AOC"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "AOC-Core"
	location "AOC-Core"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	pchheader ("pch.h")
	pchsource ("%{prj.name}/Source/pch.cpp")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/Source/",
		"%{prj.name}/Source/**Public/"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "off"
		systemversion "latest"

		defines
		{
			"AOC_CORE_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/AOC-2022"),
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/AOC-2023")
		}

	filter "configurations:Debug"
		defines "AOC_DEBUG"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "AOC_RELEASE"
		optimize "On"
		runtime "Release"

	filter "configurations:Distribution"
		defines "AOC_DISTRIBUTION"
		optimize "On"
		runtime "Release"

project "AOC-2022"
	location "AOC-2022"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	pchheader ("pch.h")
	pchsource ("%{prj.name}/Source/pch.cpp")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
		"AOC-Core/Source/",
		"AOC-Core/Source/**Public/",
		"%{prj.name}/Source/**Public/"
	}

	links
	{
		"AOC-Core"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "off"
		systemversion "latest"

		defines
		{
			
		}

	filter "configurations:Debug"
		defines "AOC_DEBUG"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "AOC_RELEASE"
		optimize "On"
		runtime "Release"

	filter "configurations:Distribution"
		defines "AOC_DISTRIBUTION"
		optimize "On"
		runtime "Release"

project "AOC-2023"
	location "AOC-2023"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	pchheader ("pch.h")
	pchsource ("%{prj.name}/Source/pch.cpp")

	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
		"AOC-Core/Source/",
		"AOC-Core/Source/**Public/",
		"%{prj.name}/Source/**Public/"
	}

	links
	{
		"AOC-Core"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "off"
		systemversion "latest"

		defines
		{
			
		}

	filter "configurations:Debug"
		defines "AOC_DEBUG"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "AOC_RELEASE"
		optimize "On"
		runtime "Release"

	filter "configurations:Distribution"
		defines "AOC_DISTRIBUTION"
		optimize "On"
		runtime "Release"
