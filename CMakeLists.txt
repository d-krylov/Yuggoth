cmake_minimum_required(VERSION 3.25)

project(Yuggoth LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

find_package(Vulkan REQUIRED COMPONENTS volk)
find_package(glfw3 3.3 REQUIRED)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

include(cmake/libraries.cmake)
include(cmake/clang-format.cmake)
include(cmake/shader-compiler.cmake)
include(cmake/helpers.cmake)

FetchContent_MakeAvailable(
  Walle
  imgui
  imguizmo
  ImGuiFileDialog
  fastgltf
  tinyobjloader
  meshoptimizer
  unordered_dense
  spdlog
  entt
  stb
)

add_subdirectory(yuggoth)

add_clang_format_target(format "build")

file(GLOB_RECURSE SHADER_FILES
  "${PROJECT_SOURCE_DIR}/assets/shaders/base/*"
  "${PROJECT_SOURCE_DIR}/assets/shaders/mesh/*"
  "${PROJECT_SOURCE_DIR}/assets/shaders/gui/*"
  "${PROJECT_SOURCE_DIR}/assets/shaders/ray/*"
)

compile_shaders("${SHADER_FILES}" "${PROJECT_BINARY_DIR}/spirv")

file(GLOB_RECURSE SHADER_SLANG_FILES
  "${PROJECT_SOURCE_DIR}/assets/shaders_slang/base/*"
)

compile_slang_shaders("${SHADER_SLANG_FILES}" "${PROJECT_BINARY_DIR}/spirv_slang")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)