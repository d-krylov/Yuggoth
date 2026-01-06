#include "shader_binding_table.h"
#include "yuggoth/graphics/graphics_context/graphics_context.h"
#include "yuggoth/core/tools/include/core.h"
#include "yuggoth/graphics/buffer/buffer.h"
#include "yuggoth/graphics/command/command_buffer.h"

namespace Yuggoth {

std::size_t GetShaderBindingTableSize(uint32_t miss, uint32_t hit, uint32_t callable) {
  auto raygen_group = 1;
  const auto &raytacing_properties = GraphicsContext::Get()->GetPhysicalDeviceRayTracingPipelineProperties();
  auto aligned_handle_size = AlignUp(raytacing_properties.shaderGroupHandleSize, raytacing_properties.shaderGroupHandleAlignment);
  auto raygen_region_size = AlignUp(raygen_group * aligned_handle_size, raytacing_properties.shaderGroupBaseAlignment);
  auto miss_region_size = AlignUp(miss * aligned_handle_size, raytacing_properties.shaderGroupBaseAlignment);
  auto hit_region_size = AlignUp(hit * aligned_handle_size, raytacing_properties.shaderGroupBaseAlignment);
  auto callable_region_size = AlignUp(callable * aligned_handle_size, raytacing_properties.shaderGroupBaseAlignment);
  return raygen_region_size + miss_region_size + hit_region_size + callable_region_size;
}

void ShaderBindingTable::SetRegions(uint32_t miss, uint32_t hit, uint32_t callable) {
  const auto &raytacing_properties = GraphicsContext::Get()->GetPhysicalDeviceRayTracingPipelineProperties();
  auto aligned_handle_size = AlignUp(raytacing_properties.shaderGroupHandleSize, raytacing_properties.shaderGroupHandleAlignment);
  auto group_alignment = raytacing_properties.shaderGroupBaseAlignment;
  auto raygen_group = 1;

  auto sbt_address = Buffer::GetBufferDeviceAddress(sbt_buffer_);

  raygen_sbt.size = AlignUp(raygen_group * aligned_handle_size, group_alignment);
  raygen_sbt.stride = aligned_handle_size;
  raygen_sbt.deviceAddress = sbt_address;

  miss_sbt_.size = AlignUp(miss * aligned_handle_size, group_alignment);
  miss_sbt_.stride = aligned_handle_size;
  miss_sbt_.deviceAddress = raygen_sbt.deviceAddress + raygen_sbt.size;

  hit_sbt_.size = AlignUp(hit * aligned_handle_size, group_alignment);
  hit_sbt_.stride = aligned_handle_size;
  hit_sbt_.deviceAddress = miss_sbt_.deviceAddress + miss_sbt_.size;

  callable_sbt_.size = AlignUp(callable * aligned_handle_size, group_alignment);
  callable_sbt_.stride = aligned_handle_size;
  callable_sbt_.deviceAddress = hit_sbt_.deviceAddress + hit_sbt_.size;
}

void CopyRegion(std::span<const std::byte> source, Buffer &destination, uint32_t count, uint32_t &offset, uint32_t &index) {
  const auto &raytacing_properties = GraphicsContext::Get()->GetPhysicalDeviceRayTracingPipelineProperties();
  auto handle_size = raytacing_properties.shaderGroupHandleSize;
  auto aligned_handle_size = AlignUp(raytacing_properties.shaderGroupHandleSize, raytacing_properties.shaderGroupHandleAlignment);
  for (auto i = 0; i < count; i++) {
    destination.SetData(source.subspan(index * handle_size, handle_size), offset);
    offset += aligned_handle_size;
    index += 1;
  }
  offset = AlignUp(offset, raytacing_properties.shaderGroupBaseAlignment);
}

void ShaderBindingTable::CreateShaderBindingTable(VkPipeline pipeline, uint32_t miss, uint32_t hit, uint32_t callable) {
  const auto &raytracing_properties = GraphicsContext::Get()->GetPhysicalDeviceRayTracingPipelineProperties();
  auto handle_size = raytracing_properties.shaderGroupHandleSize;

  auto raygen_group = 1;
  auto group_count = raygen_group + miss + hit + callable;

  std::vector<std::byte> handles(group_count * handle_size);
  VK_CHECK(vkGetRayTracingShaderGroupHandlesKHR(GraphicsContext::Get()->GetDevice(), pipeline, 0, group_count, handles.size(), handles.data()));

  auto buffer_size = GetShaderBindingTableSize(miss, hit, callable);
  auto aligned_buffer_size = AlignUp(buffer_size, raytracing_properties.shaderGroupHandleAlignment);

  Buffer staging_buffer(buffer_size, BufferUsageMaskBits::E_TRANSFER_SRC_BIT, CommonMasks::BUFFER_CPU);

  auto buffer_information = Buffer::CreateBuffer(aligned_buffer_size, CommonMasks::BUFFER_USAGE_SBT, CommonMasks::BUFFER_GPU);
  sbt_buffer_ = buffer_information.first;
  sbt_buffer_allocation_ = buffer_information.second.allocation_;

  SetRegions(miss, hit, callable);

  uint32_t offset = 0, index = 0;
  CopyRegion(handles, staging_buffer, raygen_group, offset, index);
  CopyRegion(handles, staging_buffer, miss, offset, index);
  CopyRegion(handles, staging_buffer, hit, offset, index);
  CopyRegion(handles, staging_buffer, callable, offset, index);

  CommandBuffer command_buffer(GraphicsContext::Get()->GetGraphicsQueueIndex());
  command_buffer.Begin(CommandBufferUsageMaskBits::E_ONE_TIME_SUBMIT_BIT);
  command_buffer.CommandCopyBuffer(staging_buffer.GetHandle(), sbt_buffer_, BufferCopy(0, 0, staging_buffer.GetSize()));
  command_buffer.End();
  command_buffer.Submit();
}

ShaderBindingTable::ShaderBindingTable(VkPipeline pipeline, uint32_t miss, uint32_t hit, uint32_t callable) {
  CreateShaderBindingTable(pipeline, miss, hit, callable);
}

ShaderBindingTable::~ShaderBindingTable() {
}

ShaderBindingTable::ShaderBindingTable(ShaderBindingTable &&other) noexcept {
  Swap(other);
}

ShaderBindingTable &ShaderBindingTable::operator=(ShaderBindingTable &&other) noexcept {
  Swap(other);
  return *this;
}

void ShaderBindingTable::Swap(ShaderBindingTable &other) noexcept {
  std::swap(sbt_buffer_, other.sbt_buffer_);
  std::swap(sbt_buffer_allocation_, other.sbt_buffer_allocation_);
  std::swap(raygen_sbt, other.raygen_sbt);
  std::swap(miss_sbt_, other.miss_sbt_);
  std::swap(hit_sbt_, other.hit_sbt_);
  std::swap(callable_sbt_, other.callable_sbt_);
}

const StridedDeviceAddressRegionKHR &ShaderBindingTable::GetRaygenRegion() const {
  return raygen_sbt;
}

const StridedDeviceAddressRegionKHR &ShaderBindingTable::GetMissRegion() const {
  return miss_sbt_;
}

const StridedDeviceAddressRegionKHR &ShaderBindingTable::GetHitRegion() const {
  return hit_sbt_;
}

const StridedDeviceAddressRegionKHR &ShaderBindingTable::GetCallableRegion() const {
  return callable_sbt_;
}

} // namespace Yuggoth