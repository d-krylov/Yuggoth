set(YUGGOTH_SOURCE_DIR ${PROJECT_SOURCE_DIR}/yuggoth)

set(YUGGOTH_EDITOR_SOURCES
  ${YUGGOTH_SOURCE_DIR}/editor/core/editor.cpp
  ${YUGGOTH_SOURCE_DIR}/editor/editor_windows/viewport_window.cpp
  ${YUGGOTH_SOURCE_DIR}/editor/editor_windows/hierarchy_window.cpp
)

set(YUGGOTH_GUI_SOURCES
  ${YUGGOTH_SOURCE_DIR}/gui/window/window.cpp
  ${YUGGOTH_SOURCE_DIR}/gui/window/event_kind.cpp
  ${YUGGOTH_SOURCE_DIR}/gui/imgui/imgui_renderer.cpp
  ${YUGGOTH_SOURCE_DIR}/gui/imgui/imgui_layer.cpp
)

set(YUGGOTH_MODEL_SOURCES
  ${YUGGOTH_SOURCE_DIR}/model/model.cpp
  ${YUGGOTH_SOURCE_DIR}/model/khronos_loader.cpp
)

set(YUGGOTH_SCENE_SOURCES
  ${YUGGOTH_SOURCE_DIR}/scene/core/scene.cpp
)

set(YUGGOTH_GRAPHICS_SOURCES
  ${YUGGOTH_SOURCE_DIR}/graphics/core/synchronization.cpp
  ${YUGGOTH_SOURCE_DIR}/graphics/core/structure_tools.cpp
  ${YUGGOTH_SOURCE_DIR}/graphics/core/graphics_structures.cpp
  ${YUGGOTH_SOURCE_DIR}/graphics/graphics_context/graphics_context.cpp
  ${YUGGOTH_SOURCE_DIR}/graphics/graphics_context/graphics_allocator.cpp
  ${YUGGOTH_SOURCE_DIR}/graphics/graphics_context/graphics_tools.cpp
  ${YUGGOTH_SOURCE_DIR}/graphics/pipeline/shader_module.cpp
  ${YUGGOTH_SOURCE_DIR}/graphics/pipeline/graphics_pipeline.cpp
  ${YUGGOTH_SOURCE_DIR}/graphics/pipeline/compute_pipeline.cpp
  ${YUGGOTH_SOURCE_DIR}/graphics/pipeline/ray_tracing_pipeline.cpp
  ${YUGGOTH_SOURCE_DIR}/graphics/presentation/swapchain.cpp
  ${YUGGOTH_SOURCE_DIR}/graphics/command/command_pool.cpp
  ${YUGGOTH_SOURCE_DIR}/graphics/command/command_buffer.cpp
  ${YUGGOTH_SOURCE_DIR}/graphics/image/image.cpp
  ${YUGGOTH_SOURCE_DIR}/graphics/image/image_depth.cpp
  ${YUGGOTH_SOURCE_DIR}/graphics/image/image2D.cpp
  ${YUGGOTH_SOURCE_DIR}/graphics/buffer/buffer.cpp
)

set(YUGGOTH_SOURCES
  ${YUGGOTH_GUI_SOURCES}
  ${YUGGOTH_GRAPHICS_SOURCES}
  ${YUGGOTH_EDITOR_SOURCES}
  ${YUGGOTH_MODEL_SOURCES}
  ${YUGGOTH_SCENE_SOURCES}
  ${YUGGOTH_SOURCE_DIR}/application/yuggoth.cpp
  ${YUGGOTH_SOURCE_DIR}/application/application.cpp
  ${YUGGOTH_SOURCE_DIR}/core/core.cpp
  ${Vulkan_INCLUDE_DIR}/SPIRV-Reflect/spirv_reflect.c
)

set(IMGUI_SOURCES
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imgui.cpp
)

# CONFIG

configure_file(
  ${YUGGOTH_SOURCE_DIR}/core/include/config.h.in
  ${CMAKE_BINARY_DIR}/config/yuggoth_config.h
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC 
  ${imgui_SOURCE_DIR}
)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE ${YUGGOTH_SOURCES})

message("${Vulkan_INCLUDE_DIR}")

target_include_directories(${PROJECT_NAME} 
  PUBLIC ${PROJECT_SOURCE_DIR}
  PUBLIC ${PROJECT_BINARY_DIR}
  PUBLIC ${Vulkan_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
  Vulkan::volk
  fastgltf
  glfw
  imgui
)

target_compile_definitions(${PROJECT_NAME} PUBLIC 
  VK_NO_PROTOTYPES
  GLM_ENABLE_EXPERIMENTAL
)