#include "graphics_allocator.h"
#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>
#include <cassert>
#include <print>

namespace Yuggoth {

GraphicsAllocator *GraphicsAllocator::graphics_allocator_instance_ = nullptr;

GraphicsAllocator *GraphicsAllocator::Get() {
  assert(graphics_allocator_instance_ != VK_NULL_HANDLE);
  return graphics_allocator_instance_;
}

GraphicsAllocator::GraphicsAllocator() {
  CreateAllocator();
  graphics_allocator_instance_ = this;
}

GraphicsAllocator::~GraphicsAllocator() {
  vmaDestroyAllocator(allocator_);
}

void GraphicsAllocator::CreateAllocator() {
  VmaVulkanFunctions vulkan_functions{};
  vulkan_functions.vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)vkGetInstanceProcAddr;
  vulkan_functions.vkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)vkGetDeviceProcAddr;

  VmaAllocatorCreateInfo allocator_ci{};
  allocator_ci.instance = GraphicsContext::Get()->GetInstance();
  allocator_ci.physicalDevice = GraphicsContext::Get()->GetPhysicalDevice();
  allocator_ci.device = GraphicsContext::Get()->GetDevice();
  allocator_ci.vulkanApiVersion = VK_API_VERSION_1_3;
  allocator_ci.pVulkanFunctions = &vulkan_functions;
  allocator_ci.flags = 0;

  VK_CHECK(vmaCreateAllocator(&allocator_ci, &allocator_));
}

AllocationInformation GraphicsAllocator::AllocateImage(const ImageCreateInfo &image_ci, VkImage &out_image) {
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

  statistics_.allocated_images_count_++;
  statistics_.allocated_image_memory_ += allocation_info.size;

  AllocationInformation allocation_information;
  allocation_information.allocation_ = allocation;

  return allocation_information;
}

AllocationInformation GraphicsAllocator::AllocateBuffer(const BufferCreateInfo &buffer_ci, VkBuffer &out_buffer,
                                                        AllocationCreateMask allocation_mask) {

  AllocationCreateMask cpu_bit = AllocationCreateMaskBits::E_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | //
                                 AllocationCreateMaskBits::E_HOST_ACCESS_RANDOM_BIT;

  auto has_cpu = allocation_mask.HasAnyBits(cpu_bit);

  VmaMemoryUsage memory_usage = has_cpu ? VMA_MEMORY_USAGE_AUTO_PREFER_HOST : VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;

  VmaAllocationCreateInfo vma_allocation_ci{};
  vma_allocation_ci.flags = allocation_mask.GetValue();
  vma_allocation_ci.usage = memory_usage;
  vma_allocation_ci.requiredFlags = 0;
  vma_allocation_ci.preferredFlags = 0;
  vma_allocation_ci.memoryTypeBits = 0;
  vma_allocation_ci.pool = nullptr;
  vma_allocation_ci.pUserData = nullptr;

  VmaAllocation allocation{VK_NULL_HANDLE};
  VmaAllocationInfo allocation_info{};

  VK_CHECK(vmaCreateBuffer(allocator_, buffer_ci, &vma_allocation_ci, &out_buffer, &allocation, &allocation_info));

  statistics_.allocated_buffers_count_++;
  statistics_.allocated_buffer_memory_ += allocation_info.size;

  AllocationInformation allocation_information;
  allocation_information.allocation_ = allocation;
  allocation_information.mapped_memory_ = static_cast<std::byte *>(allocation_info.pMappedData);

  return allocation_information;
}

void GraphicsAllocator::MapMemory(VmaAllocation allocation, std::byte **memory) {
  VK_CHECK(vmaMapMemory(allocator_, allocation, (void **)memory));
}

void GraphicsAllocator::UnmapMemory(VmaAllocation allocation) {
  vmaUnmapMemory(allocator_, allocation);
}

void GraphicsAllocator::CopyMemoryToAllocation(std::span<const std::byte> source, VmaAllocation destination, std::size_t offset) {
  vmaCopyMemoryToAllocation(allocator_, source.data(), destination, offset, source.size());
}

void GraphicsAllocator::DestroyImage(VkImage image, VmaAllocation vma_allocation) {
  vmaDestroyImage(allocator_, image, vma_allocation);
  statistics_.allocated_images_count_--;
}

void GraphicsAllocator::DestroyBuffer(VkBuffer buffer, VmaAllocation vma_allocation) {
  VmaAllocationInfo allocation_info{};
  vmaGetAllocationInfo(allocator_, vma_allocation, &allocation_info);
  vmaDestroyBuffer(allocator_, buffer, vma_allocation);
  statistics_.allocated_buffer_memory_ -= allocation_info.size;
  statistics_.allocated_buffers_count_--;
}

const GraphicsAllocatorStatistics &GraphicsAllocator::GetStatistics() const {
  return statistics_;
}

} // namespace Yuggoth