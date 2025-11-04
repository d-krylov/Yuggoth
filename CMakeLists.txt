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

FetchContent_MakeAvailable(
  imgui
  fastgltf
)

add_subdirectory(yuggoth)

add_clang_format_target(format "build")

file(GLOB_RECURSE SHADER_FILES
  "${PROJECT_SOURCE_DIR}/shaders/*.vert"
  "${PROJECT_SOURCE_DIR}/shaders/*.frag"
  "${PROJECT_SOURCE_DIR}/shaders/*.mesh"
)

compile_shaders("${SHADER_FILES}" "${PROJECT_BINARY_DIR}/spirv")