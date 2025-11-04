#ifndef YUGGOTH_COMMAND_BUFFER_H
#define YUGGOTH_COMMAND_BUFFER_H

#include "yuggoth/graphics/graphics_context/graphics_context.h"
#include <span>

namespace Yuggoth {

class CommandBuffer {
public:
  CommandBuffer(const VkCommandPool command_pool);

  CommandBuffer(uint32_t queue_family_index);

  ~CommandBuffer();

  CommandBuffer(const CommandBuffer &) = delete;
  CommandBuffer &operator=(const CommandBuffer &) = delete;

  CommandBuffer(CommandBuffer &&other) noexcept;
  CommandBuffer &operator=(CommandBuffer &&other) noexcept;

public:
  const VkCommandBuffer *GetPointer() const;

  void Submit();

  void Begin();
  void End();

  void CommandBeginRendering(const Extent2D &extent, std::span<const RenderingAttachmentInfo> colors);
  void CommandEndRendering();

  void Reset();

  void CommandPipelineBarrier(DependencyMask dependency_mask, std::span<const BufferMemoryBarrier2> buffer_barriers,
                              std::span<const ImageMemoryBarrier2> image_barriers);

  // PUSH
  template <typename T>
  void CommandPushConstants(VkPipelineLayout layout, ShaderStageMask stage, const T &data, uint32_t offset);
  void CommandPushDescriptorSet(VkPipelineLayout layout, uint32_t set_number, uint32_t binding, VkBuffer buffer);
  void CommandPushDescriptorSet(VkPipelineLayout layout, uint32_t set_number, uint32_t binding, VkImageView image_view, VkSampler sampler);

  // OPTIONS
  void CommandSetViewport(float x, float y, float width, float height);
  void CommandSetScissor(int32_t x, int32_t y, uint32_t width, uint32_t height);
  void CommandSetCullMode(CullModeMask mode);
  void CommandSetFrontFace(FrontFace front_face);
  void CommandEnableDepthTest(bool enabled);
  void CommandEnableDepthWrite(bool enabled);
  void CommandEnableStencilTest(bool enabled);

  // BIND
  void CommandBindPipeline(const VkPipeline pipeline, PipelineBindPoint bind_point);
  void CommandBindVertexBuffer(const VkBuffer buffer, std::size_t offset);
  void CommandBindIndexBuffer(const VkBuffer buffer, std::size_t offset, IndexType index_type);

  // DRAW
  void CommandDrawIndexed(uint32_t index_count, uint32_t instance_count, uint32_t first_index, int32_t vertex_offset, uint32_t first_instance);

  void CommandCopyBufferToImage(VkBuffer buffer, VkImage image, const Extent3D &extent);

private:
  VkCommandBuffer command_buffer_{VK_NULL_HANDLE};
  VkCommandPool command_pool_{VK_NULL_HANDLE};
  bool own_command_pool_ = false;
};

} // namespace Yuggoth

#include "command_buffer.ipp"

#endif // YUGGOTH_COMMAND_BUFFER_H