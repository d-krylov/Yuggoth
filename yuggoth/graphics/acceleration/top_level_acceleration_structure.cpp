#include "acceleration_structure.h"
#include <ranges>

namespace Yuggoth {

TransformMatrixKHR ConvertTransform(const Matrix4f &matrix) {
  TransformMatrixKHR out{};
  out.matrix[0][0] = matrix[0][0];
  out.matrix[0][1] = matrix[1][0];
  out.matrix[0][2] = matrix[2][0];
  out.matrix[0][3] = matrix[3][0];
  out.matrix[1][0] = matrix[0][1];
  out.matrix[1][1] = matrix[1][1];
  out.matrix[1][2] = matrix[2][1];
  out.matrix[1][3] = matrix[3][1];
  out.matrix[2][0] = matrix[0][2];
  out.matrix[2][1] = matrix[1][2];
  out.matrix[2][2] = matrix[2][2];
  out.matrix[2][3] = matrix[3][2];
  return out;
}

auto GetBottomLevelAccelerationStructuresAddress(VkAccelerationStructureKHR acceleration_structure) {
  AccelerationStructureDeviceAddressInfoKHR device_address_info;
  device_address_info.accelerationStructure = acceleration_structure;
  auto address = vkGetAccelerationStructureDeviceAddressKHR(GraphicsContext::Get()->GetDevice(), device_address_info);
  return address;
}

auto GetInstanceVector(std::span<const BottomLevelAccelerationStructureInstances> bottom_instances) {
  std::vector<AccelerationStructureInstanceKHR> instance_vector;
  for (const auto &[bottom_instance_index, bottom_instance] : std::views::enumerate(bottom_instances)) {
    auto buffer_device_address = GetBottomLevelAccelerationStructuresAddress(bottom_instance.acceleration_structure_);
    for (const auto &[instance_index, instance] : std::views::enumerate(bottom_instance.instances_)) {
      AccelerationStructureInstanceKHR acceleration_structure_instance;
      acceleration_structure_instance.transform = ConvertTransform(instance.transform_);
      acceleration_structure_instance.instanceCustomIndex = instance_index;
      acceleration_structure_instance.mask = 0xff;
      acceleration_structure_instance.accelerationStructureReference = buffer_device_address;
      instance_vector.emplace_back(acceleration_structure_instance);
    }
  }
  return instance_vector;
}

} // namespace Yuggoth