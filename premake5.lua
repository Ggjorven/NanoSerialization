------------------------------------------------------------------------------
-- Utilities
------------------------------------------------------------------------------
local function GetIOResult(cmd)
	local handle = io.popen(cmd) -- Open a console and execute the command.
	local output = handle:read("*a") -- Read the output.
	handle:close() -- Close the handle.

	return output:match("^%s*(.-)%s*$") -- Trim any trailing whitespace (such as newlines)
end
------------------------------------------------------------------------------

------------------------------------------------------------------------------
-- Bug fixes
------------------------------------------------------------------------------
-- Visual Studio: Bugfix for C++ Modules (same module file name per project)
-- https://github.com/premake/premake-core/issues/2177
require("vstudio")
premake.override(premake.vstudio.vc2010.elements, "clCompile", function(base, prj)
    local m = premake.vstudio.vc2010
    local calls = base(prj)

    if premake.project.iscpp(prj) then
		table.insertafter(calls, premake.xmlDeclaration,  function()
			premake.w('<ModuleDependenciesFile>$(IntDir)\\%%(RelativeDir)</ModuleDependenciesFile>')
			premake.w('<ModuleOutputFile>$(IntDir)\\%%(RelativeDir)</ModuleOutputFile>')
			premake.w('<ObjectFileName>$(IntDir)\\%%(RelativeDir)</ObjectFileName>')
		end)
    end

    return calls
end)
------------------------------------------------------------------------------

------------------------------------------------------------------------------
-- Dependencies
------------------------------------------------------------------------------
MacOSVersion = "14.5"

Dependencies =
{
	RapidYAML =
	{
		LibName = "RapidYAML",
		IncludeDir = "%{wks.location}/vendor/rapidyaml/rapidyaml/src",
	},
	RapidJSON =
	{
		IncludeDir = "%{wks.location}/vendor/rapidjson/rapidjson/include",
	},

	C4Core = 
	{
		IncludeDir = "%{wks.location}/vendor/rapidyaml/rapidyaml/ext/c4core/src",
	},

	Nano =
	{
		IncludeDir = "%{wks.location}/vendor/Nano/Nano/Nano/include",
	},
}

function ns_include_dependencies()
	for name, dep in pairs(Dependencies) do
		if dep.IncludeDir then
			includedirs { dep.IncludeDir }
		end
		if dep.LibDir then
			libdirs { dep.LibDir }
		end
	end
end

function ns_link_dependencies()
	for name, dep in pairs(Dependencies) do
		if dep.LibName then
			links { dep.LibName }
		end
	end
end
------------------------------------------------------------------------------
-- Solution
------------------------------------------------------------------------------
outputdir = "%{cfg.buildcfg}-%{cfg.system}"

workspace "NanoSerialization"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

group "Dependencies"
	include "vendor/rapidyaml"
group ""

group "NanoSerialization"
	include "NanoSerialization"
group ""

include "Sandbox"
------------------------------------------------------------------------------