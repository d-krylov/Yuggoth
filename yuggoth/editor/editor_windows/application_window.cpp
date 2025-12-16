#include "yuggoth/graphics/graphics_context/graphics_context.h"
#include "application_window.h"
#include "imgui.h"
#include <print>

namespace Yuggoth {

void ApplicationWindow::OnImGui() {
  ImGui::Begin("Application");

  const auto &memory_properties = GraphicsContext::Get()->GetPhysicalDeviceMemoryProperties();
  const auto &device_properties = GraphicsContext::Get()->GetPhysicalDeviceProperties();

  ImGui::Text("Device Name: %s", device_properties.properties.deviceName);
  ImGui::Text("Number of Memory Types: %d", memory_properties.memoryProperties.memoryTypeCount);
  ImGui::Text("Number of Memory Heaps: %d", memory_properties.memoryProperties.memoryHeapCount);

  std::span memory_heaps(memory_properties.memoryProperties.memoryHeaps, memory_properties.memoryProperties.memoryHeapCount);

  for (const auto &memory_heap : memory_heaps) {
    ImGui::Text("Memory Heap: %zu", memory_heap.size);
  }

  ImGui::End();
}

} // namespace Yuggoth