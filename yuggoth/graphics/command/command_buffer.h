#ifndef YUGGOTH_COMMAND_BUFFER_H
#define YUGGOTH_COMMAND_BUFFER_H

#include "yuggoth/graphics/core/graphics_context_types.h"
#include <string_view>
#include <span>

namespace Yuggoth {

class Pipeline;

class CommandBuffer {
public:
  CommandBuffer() = default;

  CommandBuffer(const VkCommandPool command_pool);

  CommandBuffer(QueueType queue_type);

  ~CommandBuffer();

  CommandBuffer(const CommandBuffer &) = delete;
  CommandBuffer &operator=(const CommandBuffer &) = delete;

  CommandBuffer(CommandBuffer &&other) noexcept;
  CommandBuffer &operator=(CommandBuffer &&other) noexcept;

  static VkCommandBuffer AllocateCommandBuffer(VkCommandPool command_pool, Walle::CommandBufferLevel level);

  const VkCommandBuffer *get() const;

  VkCommandBuffer GetHandle() const;

  void Submit();
  void Reset();

  void Begin(Walle::CommandBufferUsageMask usage);
  void End();

  void CommandBeginRendering(const Walle::Extent2D &extent, std::span<const Walle::RenderingAttachmentInfo> colors, //
                             const Walle::RenderingAttachmentInfo *depth = nullptr,                                 //
                             const Walle::RenderingAttachmentInfo *stencil = nullptr);

  void CommandEndRendering();

  // BARRIERS
  void CommandMemoryBarrier(Walle::PipelineStageMask2 source_stage, Walle::AccessMask2 source_access, //
                            Walle::PipelineStageMask2 destination_stage, Walle::AccessMask2 destination_access);

  void CommandPipelineBarrier(std::span<const Walle::ImageMemoryBarrier2> image_barriers, //
                              std::span<const Walle::MemoryBarrier2> memory_barriers,     //
                              std::span<const Walle::BufferMemoryBarrier2> buffer_barriers);

  // DEBUG
  void CommandBeginDebugUtilsLabel(std::string_view label_name);
  void CommandEndDebugUtilsLabel();

  // PUSH
  template <typename T> void CommandPushConstants(VkPipelineLayout layout, Walle::ShaderStageMask stage, const T &data, uint32_t offset);

  void CommandPushDescriptorSet(const Pipeline &pipeline, int32_t set, uint32_t binding, VkBuffer buffer, Walle::DescriptorType descriptor_type);

  void CommandPushDescriptorSet(VkPipelineLayout layout, uint32_t set, uint32_t binding, VkAccelerationStructureKHR tlas, Walle::PipelineBindPoint point);

  void CommandPushDescriptorSet(std::span<const Walle::DescriptorImageInfo> images, VkPipelineLayout layout, uint32_t set_number, uint32_t binding,
                                Walle::DescriptorType descriptor_type, Walle::PipelineBindPoint bind_point);

  // OPTIONS
  void CommandSetViewport(float x, float y, float width, float height);
  void CommandSetScissor(int32_t x, int32_t y, uint32_t width, uint32_t height);
  void CommandSetCullMode(Walle::CullModeMask mode);
  void CommandSetFrontFace(Walle::FrontFace front_face);
  void CommandEnableDepthTest(bool enabled);
  void CommandEnableDepthWrite(bool enabled);
  void CommandEnableStencilTest(bool enabled);

  // BIND
  void CommandBindPipeline(const VkPipeline pipeline, Walle::PipelineBindPoint bind_point);
  void CommandBindVertexBuffer(const VkBuffer buffer, std::size_t offset);
  void CommandBindIndexBuffer(const VkBuffer buffer, std::size_t offset, Walle::IndexType index_type);

  // DRAW
  void CommandDraw(uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex, uint32_t first_instance);
  void CommandDrawIndexed(uint32_t index_count, uint32_t instance_count, uint32_t first_index, int32_t vertex_offset, uint32_t first_instance);
  void CommandDrawIndexedIndirect(VkBuffer buffer, std::size_t byte_offset, uint32_t draw_count, uint32_t stride);
  void CommandDrawMeshTasks(uint32_t group_count_x, uint32_t group_count_y, uint32_t group_count_z);
  void CommandDrawMeshTasksIndirect(VkBuffer buffer, std::size_t byte_offset, uint32_t draw_count, uint32_t stride);

  // COPY
  void CommandCopyBufferToImage(VkBuffer buffer, VkImage image, const Walle::Extent3D &extent, std::size_t buffer_offset = 0);
  void CommandCopyBuffer(VkBuffer source, VkBuffer destination, const Walle::BufferCopy &buffer_copy);
  void UpdateBuffer(VkBuffer buffer, std::size_t offset, std::span<const std::byte> data);

  // RAY
  void CommandTraceRay(const Walle::StridedDeviceAddressRegionKHR &raygen, const Walle::StridedDeviceAddressRegionKHR &miss,
                       const Walle::StridedDeviceAddressRegionKHR &hit, const Walle::StridedDeviceAddressRegionKHR &callable, uint32_t width, uint32_t height,
                       uint32_t depth);

  // HELPERS
  void TransitionImageLayout(VkImage image, Walle::ImageLayout source_layout, Walle::ImageLayout destination_layout, Walle::PipelineStageMask2 source_stage,
                             Walle::PipelineStageMask2 destination_stage, Walle::AccessMask2 source_access, Walle::AccessMask2 destination_access,
                             const Walle::ImageSubresourceRange &subresource);

private:
  VkCommandBuffer command_buffer_{VK_NULL_HANDLE};
  VkCommandPool command_pool_{VK_NULL_HANDLE};
  QueueType queue_type_;
  bool own_command_pool_ = false;
};

} // namespace Yuggoth

#include "command_buffer.ipp"

#endif // YUGGOTH_COMMAND_BUFFER_H