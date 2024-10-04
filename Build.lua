-- premake5.lua
workspace "Aozora Engine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Editor"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group ""
	include "Engine/Build-Engine.lua"
group ""

include "Editor/Build-Editor.lua"
include "Game/Build-Game.lua"