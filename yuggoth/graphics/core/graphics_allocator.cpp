#include "graphics_allocator.h"
#include "yuggoth/core/tools/core.h"
#include "graphics_context.h"
#include "graphics_types.h"
#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>
#include <cassert>
#include <print>

namespace Yuggoth {

GraphicsAllocator *GraphicsAllocator::graphics_allocator_instance_ = nullptr;

GraphicsAllocator *GraphicsAllocator::Get() {
  assert(graphics_allocator_instance_ != nullptr);
  return graphics_allocator_instance_;
}

void GraphicsAllocator::Create() {
  VmaVulkanFunctions vulkan_functions{};
  vulkan_functions.vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)vkGetInstanceProcAddr;
  vulkan_functions.vkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)vkGetDeviceProcAddr;

  VmaAllocatorCreateInfo allocator_ci{};
  allocator_ci.instance = GraphicsContext::Get()->GetInstance();
  allocator_ci.physicalDevice = GraphicsContext::Get()->GetPhysicalDevice();
  allocator_ci.device = GraphicsContext::Get()->GetDevice();
  allocator_ci.vulkanApiVersion = VK_API_VERSION_1_3;
  allocator_ci.pVulkanFunctions = &vulkan_functions;
  allocator_ci.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;

  VK_CHECK(vmaCreateAllocator(&allocator_ci, &graphics_allocator_));
}

GraphicsAllocator::GraphicsAllocator() {
  Create();
  graphics_allocator_instance_ = this;
}

GraphicsAllocator::~GraphicsAllocator() {
}

ImageAllocationInformation GraphicsAllocator::AllocateImage(const Walle::ImageCreateInfo &image_ci, const AllocationCreateInformation &allocation_ci) {
  VmaAllocationCreateInfo vma_allocation_ci{};
  vma_allocation_ci.flags = 0;
  vma_allocation_ci.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
  vma_allocation_ci.requiredFlags = 0;
  vma_allocation_ci.preferredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
  vma_allocation_ci.memoryTypeBits = 0;
  vma_allocation_ci.pool = nullptr;
  vma_allocation_ci.pUserData = nullptr;

  VmaAllocation allocation{VK_NULL_HANDLE};
  VmaAllocationInfo allocation_info{};

  VkImage image = VK_NULL_HANDLE;
  VK_CHECK(vmaCreateImage(graphics_allocator_, image_ci, &vma_allocation_ci, &image, &allocation, &allocation_info));

  graphics_allocator_statistics_.allocated_images_count_++;
  graphics_allocator_statistics_.allocated_image_memory_ += allocation_info.size;

  ImageAllocationInformation image_allocation_information;
  image_allocation_information.image_handle_ = image;
  image_allocation_information.allocation_ = allocation;

  return image_allocation_information;
}

BufferAllocationInformation GraphicsAllocator::AllocateBuffer(const Walle::BufferCreateInfo &buffer_ci, const AllocationCreateInformation &allocation_ci) {
  Walle::MemoryPropertyMask host_visible = Walle::MemoryPropertyMaskBits::E_HOST_VISIBLE_BIT;

  auto has_cpu = allocation_ci.required_memory_property_.HasAnyBits(host_visible);

  VmaAllocationCreateInfo vma_allocation_ci{};
  vma_allocation_ci.flags = allocation_ci.allocation_create_mask_.GetValue();
  vma_allocation_ci.usage = has_cpu ? VMA_MEMORY_USAGE_AUTO_PREFER_HOST : VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
  vma_allocation_ci.requiredFlags = allocation_ci.required_memory_property_.GetValue();
  vma_allocation_ci.preferredFlags = allocation_ci.preferred_memory_property_.GetValue();
  vma_allocation_ci.memoryTypeBits = 0;
  vma_allocation_ci.pool = nullptr;
  vma_allocation_ci.pUserData = nullptr;

  VmaAllocation allocation{VK_NULL_HANDLE};
  VmaAllocationInfo allocation_info{};

  VkBuffer buffer = VK_NULL_HANDLE;
  VK_CHECK(vmaCreateBuffer(graphics_allocator_, buffer_ci, &vma_allocation_ci, &buffer, &allocation, &allocation_info));

  VkMemoryPropertyFlags memory_property;
  vmaGetAllocationMemoryProperties(graphics_allocator_, allocation, &memory_property);
  Walle::MemoryPropertyMask memory_property_mask(memory_property);

  graphics_allocator_statistics_.allocated_buffers_count_++;
  graphics_allocator_statistics_.allocated_buffer_memory_ += allocation_info.size;

  BufferAllocationInformation buffer_allocation_information;
  buffer_allocation_information.buffer_handle_ = buffer;
  buffer_allocation_information.allocation_ = allocation;
  buffer_allocation_information.mapped_memory_ = static_cast<uint8_t *>(allocation_info.pMappedData);
  buffer_allocation_information.memory_type_ = allocation_info.memoryType;
  buffer_allocation_information.memory_property_ = memory_property_mask;
  buffer_allocation_information.memory_size_ = allocation_info.size;

  return buffer_allocation_information;
}

void GraphicsAllocator::MapMemory(VmaAllocation allocation, uint8_t **memory) {
  VK_CHECK(vmaMapMemory(graphics_allocator_, allocation, (void **)memory));
}

void GraphicsAllocator::UnmapMemory(VmaAllocation allocation) {
  vmaUnmapMemory(graphics_allocator_, allocation);
}

void GraphicsAllocator::CopyMemoryToAllocation(std::span<const std::byte> source, VmaAllocation destination, std::size_t offset) {
  vmaCopyMemoryToAllocation(graphics_allocator_, source.data(), destination, offset, source.size());
}

void GraphicsAllocator::DestroyImage(VkImage image, VmaAllocation vma_allocation) {
  vmaDestroyImage(graphics_allocator_, image, vma_allocation);
  graphics_allocator_statistics_.allocated_images_count_--;
}

void GraphicsAllocator::DestroyBuffer(VkBuffer buffer, VmaAllocation vma_allocation) {
  VmaAllocationInfo allocation_info{};
  vmaGetAllocationInfo(graphics_allocator_, vma_allocation, &allocation_info);
  vmaDestroyBuffer(graphics_allocator_, buffer, vma_allocation);
  graphics_allocator_statistics_.allocated_buffer_memory_ -= allocation_info.size;
  graphics_allocator_statistics_.allocated_buffers_count_--;
}

const GraphicsAllocatorStatistics &GraphicsAllocator::GetGraphicsAllocatorStatistics() const {
  return graphics_allocator_statistics_;
}

} // namespace Yuggoth