#include "application.h"
#include "yuggoth/core/threading/task_system.h"

int main(int argc, char **argv) {

  Yuggoth::GraphicsContextSpecification specification;
  specification.enable_debug = true;
  specification.debug_message_severity = Yuggoth::DebugUtilsMessageSeverityMaskBitsEXT::E_ERROR_BIT_EXT;
  specification.debug_messgae_type = Yuggoth::DebugUtilsMessageTypeMaskBitsEXT::E_VALIDATION_BIT_EXT;

  Yuggoth::TaskSystem task_system;
  Yuggoth::GraphicsContext graphics_context(specification);
  Yuggoth::GraphicsAllocator graphics_allocator;
  Yuggoth::Application application;

  application.Run();

  return 0;
}