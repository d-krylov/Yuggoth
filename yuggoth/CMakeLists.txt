set(YUGGOTH_SOURCE_DIR ${PROJECT_SOURCE_DIR}/yuggoth)

collect_source_recursive("${YUGGOTH_SOURCE_DIR}/application" ".cpp" YUGGOTH_APPLICATION_SOURCES)
collect_source_recursive("${YUGGOTH_SOURCE_DIR}/mathematics" ".cpp" YUGGOTH_MATHEMATICS_SOURCES)
collect_source_recursive("${YUGGOTH_SOURCE_DIR}/core" ".cpp" YUGGOTH_CORE_SOURCES)
collect_source_recursive("${YUGGOTH_SOURCE_DIR}/asset" ".cpp" YUGGOTH_ASSET_SOURCES)
collect_source_recursive("${YUGGOTH_SOURCE_DIR}/renderer" ".cpp" YUGGOTH_RENDERER_SOURCES)
collect_source_recursive("${YUGGOTH_SOURCE_DIR}/graphics" ".cpp" YUGGOTH_GRAPHICS_SOURCES)
collect_source_recursive("${YUGGOTH_SOURCE_DIR}/editor" ".cpp" YUGGOTH_EDITOR_SOURCES)
collect_source_recursive("${YUGGOTH_SOURCE_DIR}/scene" ".cpp" YUGGOTH_SCENE_SOURCES)
collect_source_recursive("${YUGGOTH_SOURCE_DIR}/memory" ".cpp" YUGGOTH_MEMORY_SOURCES)
collect_source_recursive("${YUGGOTH_SOURCE_DIR}/gui" ".cpp" YUGGOTH_GUI_SOURCES)


set(YUGGOTH_SOURCES
  ${YUGGOTH_GUI_SOURCES}
  ${YUGGOTH_GRAPHICS_SOURCES}
  ${YUGGOTH_EDITOR_SOURCES}
  ${YUGGOTH_ASSET_SOURCES}
  ${YUGGOTH_SCENE_SOURCES}
  ${YUGGOTH_RENDERER_SOURCES}
  ${YUGGOTH_CORE_SOURCES}
  ${YUGGOTH_MATHEMATICS_SOURCES}
  ${YUGGOTH_MEMORY_SOURCES}
  ${YUGGOTH_APPLICATION_SOURCES}
  ${Vulkan_INCLUDE_DIR}/SPIRV-Reflect/spirv_reflect.c
)

set(IMGUI_SOURCES
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imgui.cpp
)

set(IMGUIZMO_SOURCES
  ${imguizmo_SOURCE_DIR}/ImSequencer.cpp
  ${imguizmo_SOURCE_DIR}/ImGuizmo.cpp
  ${imguizmo_SOURCE_DIR}/ImGradient.cpp
  ${imguizmo_SOURCE_DIR}/ImCurveEdit.cpp
  ${imguizmo_SOURCE_DIR}/GraphEditor.cpp
)

# CONFIG

configure_file(
  ${YUGGOTH_SOURCE_DIR}/core/tools/include/config.h.in
  ${CMAKE_BINARY_DIR}/config/yuggoth_config.h
)

# ImGui
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

# IMGUIZMO
add_library(imguizmo STATIC ${IMGUIZMO_SOURCES})
target_include_directories(imguizmo 
  PUBLIC ${imguizmo_SOURCE_DIR}  
  PUBLIC ${imgui_SOURCE_DIR}
)

target_link_libraries(ImGuiFileDialog PUBLIC imgui)

# YUGGOTH
add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE ${YUGGOTH_SOURCES})

message("${Vulkan_INCLUDE_DIR}")

target_include_directories(${PROJECT_NAME} 
  PUBLIC ${PROJECT_SOURCE_DIR}
  PUBLIC ${PROJECT_BINARY_DIR}
  PUBLIC ${Vulkan_INCLUDE_DIR}
  PUBLIC ${stb_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
  Vulkan::volk
  EnTT::EnTT
  fastgltf
  glfw
  imgui
  imguizmo
  ImGuiFileDialog
  tinyobjloader
  meshoptimizer
)

target_compile_definitions(${PROJECT_NAME} PUBLIC 
  VK_NO_PROTOTYPES
  GLM_ENABLE_EXPERIMENTAL
)