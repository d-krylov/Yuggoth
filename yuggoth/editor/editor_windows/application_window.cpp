#include "yuggoth/graphics/core/graphics_context.h"
#include "yuggoth/graphics/core/graphics_allocator.h"
#include <yuggoth/graphics_device/core/graphics_device.h>
#include <yuggoth/asset/model/mesh.h>
#include "application_window.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include <print>

namespace Yuggoth {

void ApplicationWindow::DrawMemoryStatistics() {
  const auto &allocator_statistics = GraphicsAllocator::Get()->GetGraphicsAllocatorStatistics();

  auto buffer_allocated_memory = allocator_statistics.allocated_buffer_memory_ / (1024.0f * 1024.0f);
  auto image_allocated_memory = allocator_statistics.allocated_image_memory_ / (1024.0f * 1024.0f);

  if (ImGui::CollapsingHeader("Memory")) {
    ImGui::Text("Number of Allocated Buffers: %zu", allocator_statistics.allocated_buffers_count_);
    ImGui::Text("Number of Allocated Images: %zu", allocator_statistics.allocated_images_count_);
    ImGui::Text("The size of the allocated memory for buffers: %f MiB", buffer_allocated_memory);
    ImGui::Text("The size of the allocated memory for images: %f MiB", image_allocated_memory);
  }
}

void ApplicationWindow::DrawSystemStatistics() {
  const auto &physical_device_properties = GraphicsContext::Get()->GetPhysicalDeviceProperties();
  if (ImGui::CollapsingHeader("System")) {
    ImGui::Text("Device Name: %s", physical_device_properties.physical_device_properties_2.properties.deviceName);
  }
}

void DrawBufferAllocatorBar(std::size_t buffer_size, const std::span<MemoryBlock> &used_chunks) {
  auto draw = ImGui::GetWindowDrawList();
  auto cursor = ImGui::GetCursorScreenPos();
  auto region = ImGui::GetContentRegionAvail();

  ImVec2 bar_size(region.x, 20.0f);

  auto free_color = IM_COL32(0, 255, 0, 255);
  auto used_color = IM_COL32(255, 0, 0, 255);

  draw->AddRectFilled(cursor, cursor + bar_size, free_color);

  for (auto &chunk : used_chunks) {
    auto chunk_begin = chunk.offset_;
    auto chunk_right = chunk.offset_ + chunk.size_;

    float x0 = cursor.x + (chunk_begin / static_cast<float>(buffer_size)) * bar_size.x;
    float x1 = cursor.x + (chunk_right / static_cast<float>(buffer_size)) * bar_size.x;

    draw->AddRectFilled(ImVec2(x0, cursor.y), ImVec2(x1, cursor.y + bar_size.y), used_color);
  }

  ImGui::Dummy(bar_size);
}

void ApplicationWindow::DrawBufferAllocator() {
  auto &vertex_allocator = GetEditorContext()->graphics_device->GetResourceManager().GetStaticBufferAllocator<Vertex>();
  auto &index_allocator = GetEditorContext()->graphics_device->GetResourceManager().GetStaticBufferAllocator<uint32_t>();

  if (ImGui::CollapsingHeader("Buffer Allocator")) {
    auto vertices = vertex_allocator.allocator_.GetAllocatorMap();
    auto indices = index_allocator.allocator_.GetAllocatorMap();

    ImGui::Text("Vertex Buffer");
    ImGui::SameLine();
    DrawBufferAllocatorBar(vertex_allocator.allocator_.GetSize(), vertices);

    ImGui::Text("Index Buffer ");
    ImGui::SameLine();
    DrawBufferAllocatorBar(index_allocator.allocator_.GetSize(), indices);
  }
}

void ApplicationWindow::OnImGui() {
  ImGui::Begin("Application");
  DrawMemoryStatistics();
  DrawSystemStatistics();
  DrawBufferAllocator();

  ImGui::End();
}

} // namespace Yuggoth