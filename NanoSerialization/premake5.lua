MacOSVersion = MacOSVersion or "14.5"

project "NanoSerialization"
	kind "StaticLib"
	language "C++"
	cppdialect "C++23"
	staticruntime "On"

	architecture "x86_64"

	warnings "Extra"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	-- Note: VS2022/Make only need the pchheader filename
	pchheader "nspch.h"
	pchsource "src/NanoSerialization/nspch.cpp"

	files
	{
		"src/NanoSerialization/**.h",
		"src/NanoSerialization/**.hpp",
		"src/NanoSerialization/**.inl",
		"src/NanoSerialization/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS"
	}

	includedirs
	{
		"src",
		"src/NanoSerialization",
	}

	ns_include_dependencies()
	ns_link_dependencies()

	filter "system:windows"
		defines "NS_PLATFORM_DESKTOP"
		defines "NS_PLATFORM_WINDOWS"
		defines "NS_PLATFORM_UNIX"
		systemversion "latest"
		staticruntime "on"

        defines
        {
            "NOMINMAX"
        }

	filter "system:linux"
		defines "NS_PLATFORM_DESKTOP"
		defines "NS_PLATFORM_LINUX"
		systemversion "latest"
		staticruntime "on"

    filter "system:macosx"
		defines "NS_PLATFORM_DESKTOP"
		defines "NS_PLATFORM_MACOS"
		defines "NS_PLATFORM_UNIX"
		defines "NS_PLATFORM_APPLE"
		systemversion(MacOSVersion)
		staticruntime "on"

	filter "action:vs*"
		buildoptions { "/Zc:preprocessor" }

	filter "action:xcode*"
		-- Note: XCode only needs the full pchheader path
		pchheader "src/NanoSerialization/nspch.h"

		-- Note: If we don't add the header files to the externalincludedirs
		-- we can't use <angled> brackets to include files.
		externalincludedirs(includedirs())

	filter "configurations:Debug"
		defines "NS_CONFIG_DEBUG"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "NS_CONFIG_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "NS_CONFIG_DIST"
		runtime "Release"
		optimize "Full"
		linktimeoptimization "on"
