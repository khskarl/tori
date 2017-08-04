PROJ_DIR = path.getabsolute("..")
DEBUG_DIR   = (path.join(PROJ_DIR, "bin/debug"))
RELEASE_DIR = (path.join(PROJ_DIR, "bin/release"))

solution "Tori"
  configurations { "Debug", "Release" }
  platforms { "x64", "x32" }
  language "C++"

project "tori"
  kind "WindowedApp"

  links {
    "GL",
    "GLEW",
    "glfw3",
    "imgui",
  }

	configuration { "linux" }
		links {
			"Xrandr",
			"Xinerama",
			"Xi",
			"Xxf86vm",
			"Xcursor",
			"X11",
			"pthread",
			"dl",
		}

  includedirs {
    path.join(PROJ_DIR, "3rdparty"),
    path.join(PROJ_DIR, "src"),
  }

  files {
    path.join(PROJ_DIR, "src/**.cpp"),
    path.join(PROJ_DIR, "src/**.hpp"),
    path.join(PROJ_DIR, "src/**.h"),
  }

  configuration { "gmake" }
    buildoptions_cpp {
      "-std=c++14",
    }

  configuration { "Debug" }
    targetdir (DEBUG_DIR)
    debugdir  (DEBUG_DIR)
    defines   "DEBUG"

  configuration { "Release" }
    targetdir (RELEASE_DIR)

  configuration {}

if os.isdir(DEBUG_DIR) == false then
  os.mkdir(DEBUG_DIR)
end

if os.isdir(RELEASE_DIR) == false then
  os.mkdir(RELEASE_DIR)
end

dofile "imgui.lua"
