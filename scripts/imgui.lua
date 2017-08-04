project "imgui"
  kind "StaticLib"

	includedirs {
		path.join(PROJ_DIR, "3rdparty"),
	}

	files {
		path.join(PROJ_DIR, "3rdparty/imgui/**.cpp"),
		path.join(PROJ_DIR, "3rdparty/imgui/**.h"),
	}

	configuration { "Debug" }
		targetdir (DEBUG_DIR)

	configuration { "Release" }
		targetdir (RELEASE_DIR)
