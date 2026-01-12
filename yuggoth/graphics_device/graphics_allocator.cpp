#include "graphics_device.h"
#include "yuggoth/core/tools/include/core.h"
#include "yuggoth/graphics/buffer/buffer_create_information.h"
#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>
#include <cassert>
#include <print>

namespace Yuggoth {

void GraphicsDevice::CreateGraphicsAllocator() {
  VmaVulkanFunctions vulkan_functions{};
  vulkan_functions.vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)vkGetInstanceProcAddr;
  vulkan_functions.vkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)vkGetDeviceProcAddr;

  VmaAllocatorCreateInfo allocator_ci{};
  allocator_ci.instance = GetInstance();
  allocator_ci.physicalDevice = GetPhysicalDevice();
  allocator_ci.device = GetDevice();
  allocator_ci.vulkanApiVersion = VK_API_VERSION_1_3;
  allocator_ci.pVulkanFunctions = &vulkan_functions;
  allocator_ci.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;

  VK_CHECK(vmaCreateAllocator(&allocator_ci, &allocator_));
}

AllocationInformation GraphicsDevice::AllocateImage(const ImageCreateInfo &image_ci, VkImage &out_image) {
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

  VK_CHECK(vmaCreateImage(allocator_, image_ci, &vma_allocation_ci, &out_image, &allocation, &allocation_info));

  graphics_allocator_statistics_.allocated_images_count_++;
  graphics_allocator_statistics_.allocated_image_memory_ += allocation_info.size;

  AllocationInformation allocation_information;
  allocation_information.allocation_ = allocation;

  return allocation_information;
}

AllocationInformation GraphicsDevice::AllocateBuffer(const BufferCreateInformation &buffer_create_information, VkBuffer &out_buffer) {
  Walle::BufferCreateInfo buffer_ci;
  buffer_ci.size = buffer_create_information.buffer_size_;
  buffer_ci.usage = buffer_create_information.buffer_usage_;
  buffer_ci.sharingMode = SharingMode::E_EXCLUSIVE;

  Walle::MemoryPropertyMask host_visible = Walle::MemoryPropertyMaskBits::E_HOST_VISIBLE_BIT;
  Walle::MemoryPropertyMask host_cached = Walle::MemoryPropertyMaskBits::E_HOST_CACHED_BIT;

  auto has_cpu = buffer_create_information.required_memory_property_.HasAnyBits(host_visible);

  VmaAllocationCreateInfo vma_allocation_ci{};
  vma_allocation_ci.flags = buffer_create_information.allocator_mask_.GetValue();
  vma_allocation_ci.usage = has_cpu ? VMA_MEMORY_USAGE_AUTO_PREFER_HOST : VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
  vma_allocation_ci.requiredFlags = buffer_create_information.required_memory_property_.GetValue();
  vma_allocation_ci.preferredFlags = buffer_create_information.preferred_memory_property_.GetValue();
  vma_allocation_ci.memoryTypeBits = 0;
  vma_allocation_ci.pool = nullptr;
  vma_allocation_ci.pUserData = nullptr;

  VmaAllocation allocation{VK_NULL_HANDLE};
  VmaAllocationInfo allocation_info{};

  VK_CHECK(vmaCreateBuffer(allocator_, buffer_ci, &vma_allocation_ci, &out_buffer, &allocation, &allocation_info));

  VkMemoryPropertyFlags memory_property;
  vmaGetAllocationMemoryProperties(allocator_, allocation, &memory_property);
  MemoryPropertyMask memory_property_mask(memory_property);

  graphics_allocator_statistics_.allocated_buffers_count_++;
  graphics_allocator_statistics_.allocated_buffer_memory_ += allocation_info.size;

  AllocationInformation allocation_information;
  allocation_information.allocation_ = allocation;
  allocation_information.mapped_memory_ = static_cast<uint8_t *>(allocation_info.pMappedData);
  allocation_information.memory_type_ = allocation_info.memoryType;
  allocation_information.memory_property_ = memory_property_mask;
  allocation_information.memory_size_ = allocation_info.size;

  return allocation_information;
}

void GraphicsDevice::MapMemory(VmaAllocation allocation, uint8_t **memory) {
  VK_CHECK(vmaMapMemory(allocator_, allocation, (void **)memory));
}

void GraphicsDevice::UnmapMemory(VmaAllocation allocation) {
  vmaUnmapMemory(allocator_, allocation);
}

void GraphicsDevice::CopyMemoryToAllocation(std::span<const std::byte> source, VmaAllocation destination, std::size_t offset) {
  vmaCopyMemoryToAllocation(allocator_, source.data(), destination, offset, source.size());
}

void GraphicsDevice::DestroyImage(VkImage image, VmaAllocation vma_allocation) {
  vmaDestroyImage(allocator_, image, vma_allocation);
  graphics_allocator_statistics_.allocated_images_count_--;
}

void GraphicsDevice::DestroyBuffer(VkBuffer buffer, VmaAllocation vma_allocation) {
  VmaAllocationInfo allocation_info{};
  vmaGetAllocationInfo(allocator_, vma_allocation, &allocation_info);
  vmaDestroyBuffer(allocator_, buffer, vma_allocation);
  graphics_allocator_statistics_.allocated_buffer_memory_ -= allocation_info.size;
  graphics_allocator_statistics_.allocated_buffers_count_--;
}

const GraphicsAllocatorStatistics &GraphicsDevice::GetGraphicsAllocatorStatistics() const {
  return graphics_allocator_statistics_;
}

} // namespace Yuggoth