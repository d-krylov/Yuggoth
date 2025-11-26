include(FetchContent)

FetchContent_Declare(
	fastgltf
	GIT_REPOSITORY https://github.com/spnda/fastgltf
	GIT_TAG v0.9.0
)

FetchContent_Declare(
	imgui
	GIT_REPOSITORY https://github.com/ocornut/imgui.git
	GIT_TAG docking
)

FetchContent_Declare(
	imguizmo
	GIT_REPOSITORY https://github.com/CedricGuillemet/ImGuizmo
	GIT_TAG master
)

FetchContent_Declare(
	entt
	GIT_REPOSITORY https://github.com/skypjack/entt
	GIT_TAG v3.15.0
)

FetchContent_Declare(
	stb
	GIT_REPOSITORY https://github.com/nothings/stb
	GIT_TAG master
)

FetchContent_Declare(
	tinyobjloader
	GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader
	GIT_TAG release
)

FetchContent_Declare(
	ImGuiFileDialog
	GIT_REPOSITORY https://github.com/aiekick/ImGuiFileDialog
	GIT_TAG master
)
