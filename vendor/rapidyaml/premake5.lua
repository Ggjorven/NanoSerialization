MacOSVersion = MacOSVersion or "14.5"

project "RapidYAML"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	-- staticruntime "Off"
	warnings "Off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"rapidyaml/src/**.h",
		"rapidyaml/src/**.hpp",
		"rapidyaml/src/**.cpp",

		"rapidyaml/ext/c4core/src/c4/**.h",
		"rapidyaml/ext/c4core/src/c4/**.hpp",
		"rapidyaml/ext/c4core/src/c4/**.cpp",
	}

	removefiles 
	{
		"rapidyaml/ext/c4core/src/c4/ext/**.h",
		"rapidyaml/ext/c4core/src/c4/ext/**.hpp",
		"rapidyaml/ext/c4core/src/c4/ext/**.cpp",
	}

	files
	{
		"rapidyaml/ext/c4core/src/c4/ext/fast_float/include/**.h",
		"rapidyaml/ext/c4core/src/c4/ext/fast_float/include/**.hpp",
		"rapidyaml/ext/c4core/src/c4/ext/fast_float/include/**.cpp",
	}

	includedirs
    {
        "rapidyaml/src",
        "rapidyaml/ext/c4core/src",
		"rapidyaml/ext/c4core/src/c4/ext/fast_float/include",
    }

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

	filter "system:linux"
		systemversion "latest"
		staticruntime "On"

	filter "system:macosx"
		systemversion(MacOSVersion)
		staticruntime "On"

	filter "action:xcode*"
		-- Note: If we don't add the header files to the externalincludedirs
		-- we can't use <angled> brackets to include files.
		externalincludedirs(includedirs())

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		runtime "Release"
		optimize "Full"
		linktimeoptimization "On"