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

void ApplicationWindow::OnImGui() {
  ImGui::Begin("Application");
  DrawMemoryStatistics();
  DrawSystemStatistics();
  ImGui::End();
}

} // namespace Yuggoth