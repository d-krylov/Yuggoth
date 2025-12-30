#include "acceleration_structure.h"
#include "yuggoth/graphics/buffer/buffer.h"
#include "yuggoth/graphics/command/command_buffer.h"
#include <algorithm>
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

auto GetInstanceVector(std::span<const BLASInstances> bottom_instances) {
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

void AccelerationStructure::BuildTLAS(std::span<const BLASInstances> blas_instances, const AccelerationSpecification &specification) {
  uint32_t primitive_count = 0;
  std::ranges::for_each(blas_instances, [&](const auto &instances) { primitive_count += instances.size(); }, &BLASInstances::instances_);

  auto size = sizeof(AccelerationStructureInstanceKHR) * primitive_count;

  Buffer instance_buffer(size, CommonMasks::BUFFER_USAGE_SOURCES_AS, CommonMasks::BUFFER_CPU);

  auto instance_vector = GetInstanceVector(blas_instances);
  instance_buffer.SetData<AccelerationStructureInstanceKHR>(instance_vector);

  std::array<AccelerationStructureGeometryKHR, 1> geometries;
  geometries[0].geometryType = GeometryTypeKHR::E_INSTANCES_KHR;
  geometries[0].geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
  geometries[0].geometry.instances.data.deviceAddress = instance_buffer.GetDeviceAddress();

  auto sizes = GetAccelerationStructureSize(primitive_count);

  auto type = AccelerationStructureTypeKHR::E_TOP_LEVEL_KHR;

  BufferUsageMask scratch_usage = BufferUsageMaskBits::E_SHADER_DEVICE_ADDRESS_BIT | BufferUsageMaskBits::E_STORAGE_BUFFER_BIT;
  Buffer scrath_buffer(sizes.buildScratchSize, scratch_usage, AllocationCreateMaskBits::E_DEDICATED_MEMORY_BIT);

  acceleration_structure_ = CreateAccelerationStructure(specification.buffer_, type, specification.buffer_offset_, specification.structure_size_);
  auto geometry_info = GetBuildGeometryInformation(geometries, scrath_buffer.GetDeviceAddress(), acceleration_structure_, type);

  AccelerationStructureBuildRangeInfoKHR build_range;
  build_range.firstVertex = 0;
  build_range.primitiveOffset = 0;
  build_range.primitiveCount = primitive_count;

  const VkAccelerationStructureBuildRangeInfoKHR *build_range_pointer = build_range;

  CommandBuffer command_buffer(GraphicsContext::Get()->GetGraphicsQueueIndex());
  command_buffer.Begin(CommandBufferUsageMaskBits::E_ONE_TIME_SUBMIT_BIT);
  vkCmdBuildAccelerationStructuresKHR(command_buffer.GetHandle(), 1, geometry_info, &build_range_pointer);
  command_buffer.End();
  command_buffer.Submit();
}

} // namespace Yuggoth