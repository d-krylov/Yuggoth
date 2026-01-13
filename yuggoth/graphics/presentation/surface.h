#ifndef YUGGOTH_SURFACE_H
#define YUGGOTH_SURFACE_H

#include "walle/walle.h"

namespace Yuggoth {

class Surface {
public:
  Surface(VkSurfaceKHR surface);

  ~Surface();

  Walle::SurfaceCapabilitiesKHR GetSurfaceCapabilities() const;

  Walle::SurfaceFormatKHR GetSurfaceFormat() const;
  Walle::PresentModeKHR GetPresentMode() const;

  VkSurfaceKHR GetHandle() const;

private:
  VkSurfaceKHR surface_{VK_NULL_HANDLE};
  Walle::SurfaceFormatKHR surface_format_;
  Walle::PresentModeKHR present_mode_;
};

} // namespace Yuggoth

#endif // YUGGOTH_SURFACE_H