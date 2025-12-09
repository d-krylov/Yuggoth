#include "yuggoth/application/application.h"
#include "application_window.h"
#include "imgui.h"
#include <print>

namespace Yuggoth {

void ApplicationWindow::OnImGui() {
  ImGui::Begin("Application");

  auto memory_manager = Application::Get()->GetDeviceMemoryManager();
  auto &memory_properties = memory_manager->GetPhysicalDeviceMemoryProperties();

  ImGui::Text("Number of Memory Types: %d", memory_properties.memoryTypeCount);
  ImGui::Text("Number of Memory Heaps: %d", memory_properties.memoryHeapCount);

  std::span memory_heaps(memory_properties.memoryHeaps, memory_properties.memoryHeapCount);

  for (const auto &memory_heap : memory_heaps) {
    ImGui::Text("Memory Heap: %zu", memory_heap.size);
  }

  ImGui::End();
}

} // namespace Yuggoth