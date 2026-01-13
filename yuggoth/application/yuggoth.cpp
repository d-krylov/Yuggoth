#include "application.h"
#include "yuggoth/core/threading/task_system.h"
#include "yuggoth/graphics/core/graphics_context.h"
#include "yuggoth/graphics/core/graphics_allocator.h"

VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(Walle::DebugUtilsMessageSeverityMaskBitsEXT severity,  //
                                             Walle::DebugUtilsMessageTypeMaskEXT type,              //
                                             const Walle::DebugUtilsMessengerCallbackDataEXT *data, //
                                             void *user_data) {
  std::println("Validation Layer: {0}", data->pMessage);
  return false;
}

int main(int argc, char **argv) {

  Yuggoth::GraphicsContextCreateInformation graphics_context_ci;
  graphics_context_ci.enable_debug = true;
  graphics_context_ci.debug_callback_ = DebugCallback;
  graphics_context_ci.debug_message_severity_ = Walle::DebugUtilsMessageSeverityMaskBitsEXT::E_ERROR_BIT_EXT |
                                                Walle::DebugUtilsMessageSeverityMaskBitsEXT::E_VERBOSE_BIT_EXT |
                                                Walle::DebugUtilsMessageSeverityMaskBitsEXT::E_WARNING_BIT_EXT;
  graphics_context_ci.debug_message_type_ =
    Yuggoth::DebugUtilsMessageTypeMaskBitsEXT::E_VALIDATION_BIT_EXT | Walle::DebugUtilsMessageTypeMaskBitsEXT::E_GENERAL_BIT_EXT;

  graphics_context_ci.required_extensions_ = Yuggoth::Window::GetRequiredInstanceExtensions();

  Yuggoth::TaskSystem task_system;
  Yuggoth::GraphicsContext graphics_context(graphics_context_ci);
  Yuggoth::GraphicsAllocator graphics_allocator;
  Yuggoth::Application application;

  application.Run();

  return 0;
}