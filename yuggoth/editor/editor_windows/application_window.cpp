#include "yuggoth/graphics/graphics_context/graphics_context.h"
#include "application_window.h"
#include "imgui.h"
#include <print>

namespace Yuggoth {

void ApplicationWindow::DrawMemoryStatistics() {
  const auto &allocator_statistics = GraphicsAllocator::Get()->GetStatistics();

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
  const auto &memory_properties = GraphicsContext::Get()->GetPhysicalDeviceMemoryProperties();
  const auto &device_properties = GraphicsContext::Get()->GetPhysicalDeviceProperties();

  if (ImGui::CollapsingHeader("System")) {
    ImGui::Text("Device Name: %s", device_properties.properties.deviceName);
  }
}

void DrawAllocatorVisualization(size_t all_size, const std::span<MemoryBlock> &allocated_chunks) {
  auto size = ImVec2(800, 20);
  auto draw_list = ImGui::GetWindowDrawList();
  auto cursor = ImGui::GetCursorScreenPos();

  draw_list->AddRectFilled(cursor, ImVec2(cursor.x + size.x, cursor.y + size.y), IM_COL32(0, 255, 0, 255));

  for (auto &chunk : allocated_chunks) {
    float x0 = cursor.x + (chunk.offset_ / (float)all_size) * size.x;
    float x1 = cursor.x + ((chunk.offset_ + chunk.size_) / (float)all_size) * size.x;
    draw_list->AddRectFilled(ImVec2(x0, cursor.y), ImVec2(x1, cursor.y + size.y), IM_COL32(255, 0, 0, 255));
  }

  ImGui::Dummy(size);
}

void ApplicationWindow::OnImGui() {
  ImGui::Begin("Application");
  DrawMemoryStatistics();
  DrawSystemStatistics();

  auto vertex_allocator = GetEditorContext()->buffer_manager_->GetVertexAllocator().GetAllocator();

  auto vertex_blocks = vertex_allocator->GetAllocatorMap();

  DrawAllocatorVisualization(vertex_allocator->GetSize(), vertex_blocks);

  ImGui::End();
}

} // namespace Yuggoth