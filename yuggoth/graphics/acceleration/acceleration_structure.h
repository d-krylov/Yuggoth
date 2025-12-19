#ifndef YUGGOTH_ACCELERATION_STRUCTURE_H
#define YUGGOTH_ACCELERATION_STRUCTURE_H

#include "yuggoth/graphics/graphics_context/graphics_context.h"
#include "yuggoth/mathematics/include/mathematics_types.h"
#include <span>

namespace Yuggoth {

struct InstanceData {
  Matrix4f transform_{1.0f};
};

struct BottomLevelAccelerationStructureInstances {
  std::vector<InstanceData> instances_;
  VkAccelerationStructureKHR acceleration_structure_{VK_NULL_HANDLE};
};

} // namespace Yuggoth

#endif // YUGGOTH_ACCELERATION_STRUCTURE_H