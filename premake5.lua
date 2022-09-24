workspace "RayTracing"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "RayTracing"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
   include "vendor/imgui"
   include "vendor/glfw"
   include "vendor/glad"
group ""

project "RayTracing"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "vendor/imgui",
      "vendor/glfw/include",
      "vendor/glm",
      "vendor/glad",
   }

   links
   {
      "GLFW",
      "Glad",
      "ImGui",
   }

   defines
   {
      "GLFW_INCLUDE_NONE",
      "_CRT_SECURE_NO_WARNINGS"
   }

   targetdir ("bin/" .. outputdir .. "/%{prj.name}")
   objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"

   filter "configurations:Debug"
      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      kind "WindowedApp"
      runtime "Release"
      optimize "On"
      symbols "Off"