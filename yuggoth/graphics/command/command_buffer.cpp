#include "command_buffer.h"
#include "command_pool.h"
#include "yuggoth/graphics/synchronization/fence.h"

namespace Yuggoth {

CommandBuffer::CommandBuffer(const VkCommandPool command_pool) : command_pool_(command_pool), own_command_pool_(false) {
  command_buffer_ = CommandBuffer::AllocateCommandBuffer(command_pool_, CommandBufferLevel::E_PRIMARY);
}

CommandBuffer::CommandBuffer(uint32_t queue_family_index) : own_command_pool_(true) {
  command_pool_ = CommandPool::CreateCommandPool(queue_family_index);
  command_buffer_ = CommandBuffer::AllocateCommandBuffer(command_pool_, CommandBufferLevel::E_PRIMARY);
}

CommandBuffer::~CommandBuffer() {
  vkDestroyCommandPool(GraphicsContext::Get()->GetDevice(), own_command_pool_ ? command_pool_ : VK_NULL_HANDLE, nullptr);
}

CommandBuffer::CommandBuffer(CommandBuffer &&other) noexcept {
  command_buffer_ = std::exchange(other.command_buffer_, VK_NULL_HANDLE);
  command_pool_ = std::exchange(other.command_pool_, VK_NULL_HANDLE);
  own_command_pool_ = std::exchange(other.own_command_pool_, false);
}

CommandBuffer &CommandBuffer::operator=(CommandBuffer &&other) noexcept {
  std::swap(command_buffer_, other.command_buffer_);
  std::swap(command_pool_, other.command_pool_);
  std::swap(own_command_pool_, other.own_command_pool_);
  return *this;
}

VkCommandBuffer CommandBuffer::AllocateCommandBuffer(VkCommandPool command_pool, CommandBufferLevel level) {
  CommandBufferAllocateInfo command_buffer_ai;
  command_buffer_ai.commandPool = command_pool;
  command_buffer_ai.level = level;
  command_buffer_ai.commandBufferCount = 1;

  VkCommandBuffer command_buffer = VK_NULL_HANDLE;
  VK_CHECK(vkAllocateCommandBuffers(GraphicsContext::Get()->GetDevice(), command_buffer_ai, &command_buffer));
  return command_buffer;
}

VkCommandBuffer CommandBuffer::GetHandle() const {
  return command_buffer_;
}

void CommandBuffer::Submit() {
  SubmitInfo submit_info;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = get();

  Fence fence;
  VK_CHECK(vkQueueSubmit(GraphicsContext::Get()->GetGraphicsQueue(), 1, submit_info, fence.GetHandle()));
  fence.Wait();
}

const VkCommandBuffer *CommandBuffer::get() const {
  return &command_buffer_;
}

void CommandBuffer::Begin(CommandBufferUsageMask usage) {
  CommandBufferBeginInfo command_buffer_bi;
  command_buffer_bi.flags = usage;
  vkBeginCommandBuffer(command_buffer_, command_buffer_bi);
}

void CommandBuffer::Reset() {
  vkResetCommandBuffer(command_buffer_, 0);
}

void CommandBuffer::CommandBeginRendering(const Extent2D &extent, std::span<const RenderingAttachmentInfo> colors,
                                          const RenderingAttachmentInfo *depth, const RenderingAttachmentInfo *stencil) {
  RenderingInfo rendering_info;
  rendering_info.renderArea.offset = {0, 0};
  rendering_info.renderArea.extent = extent;
  rendering_info.layerCount = 1;
  rendering_info.colorAttachmentCount = colors.size();
  rendering_info.pColorAttachments = colors.data();
  rendering_info.pDepthAttachment = depth;
  rendering_info.pStencilAttachment = stencil;

  vkCmdBeginRendering(command_buffer_, rendering_info);
}

void CommandBuffer::End() {
  vkEndCommandBuffer(command_buffer_);
}

void CommandBuffer::CommandEndRendering() {
  vkCmdEndRendering(command_buffer_);
}

void CommandBuffer::CommandPipelineBarrier(std::span<const ImageMemoryBarrier2> image_barriers, std::span<const MemoryBarrier2> memory_barriers,
                                           std::span<const BufferMemoryBarrier2> buffer_barriers) {
  DependencyInfo dependency_info;
  {
    dependency_info.dependencyFlags = DependencyMaskBits::E_BY_REGION_BIT;
    dependency_info.bufferMemoryBarrierCount = buffer_barriers.size();
    dependency_info.pBufferMemoryBarriers = buffer_barriers.data();
    dependency_info.imageMemoryBarrierCount = image_barriers.size();
    dependency_info.pImageMemoryBarriers = image_barriers.data();
    dependency_info.memoryBarrierCount = memory_barriers.size();
    dependency_info.pMemoryBarriers = memory_barriers.data();
  }
  vkCmdPipelineBarrier2(command_buffer_, dependency_info);
}

// PUSH

void CommandBuffer::CommandPushDescriptorSet(VkPipelineLayout layout, uint32_t set_number, uint32_t binding, VkBuffer buffer) {
  DescriptorBufferInfo descriptor_bi;
  {
    descriptor_bi.buffer = buffer;
    descriptor_bi.offset = 0;
    descriptor_bi.range = VK_WHOLE_SIZE;
  }

  WriteDescriptorSet write_descriptor_set;
  {
    write_descriptor_set.dstBinding = binding;
    write_descriptor_set.dstArrayElement = 0;
    write_descriptor_set.descriptorType = DescriptorType::E_STORAGE_BUFFER;
    write_descriptor_set.descriptorCount = 1;
    write_descriptor_set.pBufferInfo = &descriptor_bi;
  }
  vkCmdPushDescriptorSetKHR(command_buffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, set_number, 1, write_descriptor_set);
}

void CommandBuffer::CommandPushDescriptorSet(VkPipelineLayout layout, uint32_t set, uint32_t binding, VkImageView image_view, VkSampler sampler) {
  DescriptorImageInfo descriptor_ii;
  {
    descriptor_ii.sampler = sampler;
    descriptor_ii.imageView = image_view;
    descriptor_ii.imageLayout = ImageLayout::E_SHADER_READ_ONLY_OPTIMAL;
  }

  WriteDescriptorSet write_descriptor_set;
  {
    write_descriptor_set.dstBinding = binding;
    write_descriptor_set.dstArrayElement = 0;
    write_descriptor_set.descriptorType = DescriptorType::E_COMBINED_IMAGE_SAMPLER;
    write_descriptor_set.descriptorCount = 1;
    write_descriptor_set.pImageInfo = &descriptor_ii;
  }
  vkCmdPushDescriptorSetKHR(command_buffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, set, 1, write_descriptor_set);
}

void CommandBuffer::CommandSetViewport(float x, float y, float width, float height) {
  Viewport viewport{x, y, width, height, 0.0f, 1.0f};
  vkCmdSetViewport(command_buffer_, 0, 1, viewport);
}

void CommandBuffer::CommandSetScissor(int32_t x, int32_t y, uint32_t width, uint32_t height) {
  Rect2D scissor{x, y, width, height};
  vkCmdSetScissor(command_buffer_, 0, 1, scissor);
}

void CommandBuffer::CommandSetCullMode(CullModeMask mode) {
  vkCmdSetCullMode(command_buffer_, mode.GetValue());
}

void CommandBuffer::CommandSetFrontFace(FrontFace front_face) {
  vkCmdSetFrontFace(command_buffer_, static_cast<VkFrontFace>(front_face));
}

void CommandBuffer::CommandEnableDepthTest(bool enabled) {
  vkCmdSetDepthTestEnable(command_buffer_, enabled);
}

void CommandBuffer::CommandEnableDepthWrite(bool enabled) {
  vkCmdSetDepthWriteEnable(command_buffer_, enabled);
}

void CommandBuffer::CommandEnableStencilTest(bool enabled) {
  vkCmdSetStencilTestEnable(command_buffer_, enabled);
}

// DRAW
void CommandBuffer::CommandDrawIndexed(uint32_t indices, uint32_t instances, uint32_t first_index, int32_t vertex_offset, uint32_t first_instance) {
  vkCmdDrawIndexed(command_buffer_, indices, instances, first_index, vertex_offset, first_instance);
}

// BIND

void CommandBuffer::CommandBindPipeline(const VkPipeline pipeline, PipelineBindPoint bind_point) {
  vkCmdBindPipeline(command_buffer_, static_cast<VkPipelineBindPoint>(bind_point), pipeline);
}

void CommandBuffer::CommandBindVertexBuffer(const VkBuffer buffer, std::size_t offset) {
  vkCmdBindVertexBuffers(command_buffer_, 0, 1, &buffer, &offset);
}

void CommandBuffer::CommandBindIndexBuffer(const VkBuffer buffer, std::size_t offset, IndexType index_type) {
  vkCmdBindIndexBuffer(command_buffer_, buffer, offset, static_cast<VkIndexType>(index_type));
}

// COPY
void CommandBuffer::CommandCopyBufferToImage(VkBuffer buffer, VkImage image, const Extent3D &extent) {
  ImageSubresourceLayers subresource_layers;
  subresource_layers.aspectMask = ImageAspectMaskBits::E_COLOR_BIT;
  subresource_layers.mipLevel = 0;
  subresource_layers.baseArrayLayer = 0;
  subresource_layers.layerCount = 1;

  BufferImageCopy buffer_image_copy;
  buffer_image_copy.bufferOffset = 0;
  buffer_image_copy.bufferRowLength = 0;
  buffer_image_copy.bufferImageHeight = 0;
  buffer_image_copy.imageSubresource = subresource_layers;
  buffer_image_copy.imageOffset = Offset3D(0, 0, 0);
  buffer_image_copy.imageExtent = extent;

  vkCmdCopyBufferToImage(command_buffer_, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, buffer_image_copy);
}

void CommandBuffer::CommandCopyBuffer(VkBuffer source, VkBuffer destination, const BufferCopy &buffer_copy) {
  vkCmdCopyBuffer(command_buffer_, source, destination, 1, buffer_copy);
}

void CommandBuffer::CommandBeginDebugUtilsLabel(std::string_view label_name) {
  DebugUtilsLabelEXT debug_utils_label;
  debug_utils_label.color[0] = 1.0f;
  debug_utils_label.color[3] = 1.0f;
  debug_utils_label.pLabelName = label_name.data();
  vkCmdBeginDebugUtilsLabelEXT(command_buffer_, debug_utils_label);
}

void CommandBuffer::CommandEndDebugUtilsLabel() {
  vkCmdEndDebugUtilsLabelEXT(command_buffer_);
}

// HELPERS
void CommandBuffer::TransitionImageLayout(VkImage image, ImageLayout source_layout, ImageLayout destination_layout, PipelineStageMask2 source_stage,
                                          PipelineStageMask2 destination_stage, AccessMask2 source_access, AccessMask2 destination_access,
                                          const ImageSubresourceRange &subresource) {
  std::array<ImageMemoryBarrier2, 1> image_memory_barriers;

  image_memory_barriers[0].srcStageMask = source_stage;
  image_memory_barriers[0].srcAccessMask = source_access;
  image_memory_barriers[0].dstStageMask = destination_stage;
  image_memory_barriers[0].dstAccessMask = destination_access;
  image_memory_barriers[0].oldLayout = source_layout;
  image_memory_barriers[0].newLayout = destination_layout;
  image_memory_barriers[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  image_memory_barriers[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  image_memory_barriers[0].image = image;
  image_memory_barriers[0].subresourceRange = subresource;

  CommandPipelineBarrier(image_memory_barriers);
}

} // namespace Yuggoth