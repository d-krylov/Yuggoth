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

FetchContent_MakeAvailable(
  imgui
  fastgltf
)

add_subdirectory(yuggoth)

add_clang_format_target(format "build")
