#ifndef YUGGOTH_GRAPHICS_TYPES_H
#define YUGGOTH_GRAPHICS_TYPES_H

#include "structure_tools.h"
#include "yuggoth/core/allocators/virtual_allocator_types.h"

namespace Yuggoth {

ImageAspectMask GetAspectMask(Format format);
AccessMask2 GetAccessMaskFromLayout(ImageLayout layout, bool destination);
PipelineStageMask2 GetPipelineStageMaskFromLayout(ImageLayout layout, bool destination);

} // namespace Yuggoth

#endif // YUGGOTH_GRAPHICS_TYPES_H