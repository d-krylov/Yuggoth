#include "application.h"
#include "yuggoth/core/threading/task_system.h"

int main(int argc, char **argv) {

  Yuggoth::GraphicsDeviceSpecification specification;
  specification.enable_debug = false;
  specification.debug_message_severity = Walle::DebugUtilsMessageSeverityMaskBitsEXT::E_ERROR_BIT_EXT |
                                         Walle::DebugUtilsMessageSeverityMaskBitsEXT::E_VERBOSE_BIT_EXT |
                                         Walle::DebugUtilsMessageSeverityMaskBitsEXT::E_WARNING_BIT_EXT;
  specification.debug_messgae_type =
    Yuggoth::DebugUtilsMessageTypeMaskBitsEXT::E_VALIDATION_BIT_EXT | Walle::DebugUtilsMessageTypeMaskBitsEXT::E_GENERAL_BIT_EXT;

  Yuggoth::TaskSystem task_system;
  Yuggoth::GraphicsDevice graphics_context(specification);
  Yuggoth::Application application;

  application.Run();

  return 0;
}