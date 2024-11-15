project "Engine"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.h",
   "Source/**.cpp",
   "Source/**.c",
   "Vendor/include/imgui/*.cpp",
   "Resources/Shaders/*"}

   includedirs
   {
      "Source",
	  "Vendor/include/",
   }
   
   libdirs{"Vendor/libs/GLFW", "Vendor/libs/assimp"}
   
   
   
    links
   {
	  "glfw3.lib",
	  "opengl32.lib",
	  "assimp-vc143-mtd.lib"
   }
   
   
   
   vpaths {
      ["Systems/Renderers"] = { "Systems/renderers/*.cpp", "Systems/renderers/*.h" },
	  ["Systems/Renderers/Opengl"] = { "Systems/renderers/Opengl/*.cpp", "Systems/renderers/Opengl/*.h" },
	  ["Systems/Events"] = { "Systems/Events/*.cpp", "Systems/Events/*.h" },
      ["Systems/Windows"] = { "Systems/windows/*.cpp", "Systems/windows/*.h" },
	  ["Systems/ECS"] = { "Systems/ECS/*.cpp", "Systems/ECS/*.h" },
	  ["Systems/ECS/Components"] = { "Systems/ECS/Components/*.cpp", "Systems/ECS/Components/*.h" },
	  ["Systems/Project"] = { "Systems/Project/*.cpp", "Systems/Project/*.h" },
	  ["Systems/Scene"] = { "Systems/Scene/*.cpp", "Systems/Scene/*.h" },
	  ["Imgui"] = { "Vendor/include/imgui/*.cpp"},
      ["Engine"] = { "Engine/*.cpp", "Engine/*.h" },
	  ["Resources"] = { "Resources/*"}
	  }


   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"