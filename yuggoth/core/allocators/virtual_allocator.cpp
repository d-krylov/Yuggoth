#include "virtual_allocator.h"
#include <utility>

namespace Yuggoth {

VirtualAllocator::VirtualAllocator(std::size_t size) : size_(size) {
  VmaVirtualBlockCreateInfo block_info{};
  block_info.size = size_;
  auto result = vmaCreateVirtualBlock(&block_info, &virtual_block_);
}

VirtualAllocator::~VirtualAllocator() {
  Destroy();
}

VirtualAllocator::VirtualAllocator(VirtualAllocator &&other) noexcept {
  Swap(other);
}

VirtualAllocator &VirtualAllocator::operator=(VirtualAllocator &&other) noexcept {
  Swap(other);
  return *this;
}

void VirtualAllocator::Swap(VirtualAllocator &other) noexcept {
  std::swap(size_, other.size_);
  std::swap(virtual_block_, other.virtual_block_);
  std::swap(allocations_, other.allocations_);
}

std::size_t VirtualAllocator::Allocate(std::size_t size, std::size_t alignment, Walle::VirtualAllocationCreateMask mask) {
  VmaVirtualAllocationCreateInfo allocation_ci{};
  allocation_ci.size = size;
  allocation_ci.flags = mask.GetValue();
  allocation_ci.alignment = alignment;
  VmaVirtualAllocation virtual_allocation;
  std::size_t offset{0};
  auto result = vmaVirtualAllocate(virtual_block_, &allocation_ci, &virtual_allocation, &offset);
  allocations_.emplace(offset, virtual_allocation);
  return offset;
}

void VirtualAllocator::Destroy() {
  vmaDestroyVirtualBlock(virtual_block_);
}

void VirtualAllocator::Clear() {
  vmaClearVirtualBlock(virtual_block_);
}

void VirtualAllocator::Free(std::size_t offset) {
  vmaVirtualFree(virtual_block_, allocations_.at(offset));
  allocations_.erase(offset);
}

std::size_t VirtualAllocator::GetSize() const {
  return size_;
}

std::vector<MemoryBlock> VirtualAllocator::GetAllocatorMap() const {
  std::vector<MemoryBlock> out;
  VmaVirtualAllocationInfo allocation_info{};
  for (const auto &allocation : allocations_) {
    vmaGetVirtualAllocationInfo(virtual_block_, allocation.second, &allocation_info);
    out.emplace_back(allocation_info.offset, allocation_info.size);
  }
  return out;
}

} // namespace Yuggoth