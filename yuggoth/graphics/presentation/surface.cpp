#include "surface.h"
#include "yuggoth/core/tools/type_tools.h"
#include "yuggoth/graphics/core/graphics_context.h"
#include "yuggoth/graphics/core/structure_tools.h"
#include <algorithm>

namespace Yuggoth {

Walle::SurfaceFormatKHR SelectSurfaceFormat(VkSurfaceKHR surface, std::span<const Walle::SurfaceFormatKHR> required_formats) {
  auto physical_device = GraphicsContext::Get()->GetPhysicalDevice();
  auto supported_formats = Enumerate<Walle::SurfaceFormatKHR>(vkGetPhysicalDeviceSurfaceFormatsKHR, physical_device, surface);
  auto it = std::ranges::find_first_of(supported_formats, required_formats);
  return (it != supported_formats.end()) ? *it : supported_formats[0];
}

Walle::PresentModeKHR SelectPresentMode(VkSurfaceKHR surface, std::span<const Walle::PresentModeKHR> required_present_modes) {
  auto physical_device = GraphicsContext::Get()->GetPhysicalDevice();
  auto supported_modes = Enumerate<Walle::PresentModeKHR>(vkGetPhysicalDeviceSurfacePresentModesKHR, physical_device, surface);
  auto it = std::ranges::find_first_of(required_present_modes, supported_modes);
  return *it;
}

Walle::SurfaceCapabilitiesKHR Surface::GetSurfaceCapabilities() const {
  Walle::SurfaceCapabilitiesKHR surface_capabilities;
  VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(GraphicsContext::Get()->GetPhysicalDevice(), surface_, surface_capabilities));
  return surface_capabilities;
}

Surface::Surface(VkSurfaceKHR surface) : surface_(surface) {
  std::array required_present_modes = {Walle::PresentModeKHR::E_MAILBOX_KHR, Walle::PresentModeKHR::E_IMMEDIATE_KHR, Walle::PresentModeKHR::E_FIFO_KHR};
  std::vector<Walle::SurfaceFormatKHR> required_formats{{Walle::Format::E_R8G8B8A8_SRGB, Walle::ColorSpaceKHR::E_SRGB_NONLINEAR_KHR},
                                                        {Walle::Format::E_B8G8R8A8_SRGB, Walle::ColorSpaceKHR::E_SRGB_NONLINEAR_KHR}};

  surface_format_ = SelectSurfaceFormat(surface_, required_formats);
  present_mode_ = SelectPresentMode(surface_, required_present_modes);
}

Surface::~Surface() {
}

Walle::SurfaceFormatKHR Surface::GetSurfaceFormat() const {
  return surface_format_;
}

Walle::PresentModeKHR Surface::GetPresentMode() const {
  return present_mode_;
}

VkSurfaceKHR Surface::GetHandle() const {
  return surface_;
}

} // namespace Yuggoth