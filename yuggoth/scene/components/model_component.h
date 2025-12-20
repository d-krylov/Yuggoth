#ifndef YUGGOTH_MODEL_COMPONENT_H
#define YUGGOTH_MODEL_COMPONENT_H

#include "yuggoth/asset/include/model.h"
#include "yuggoth/asset/include/buffer_owning_model.h"
#include "yuggoth/core/intrusive/intrusive_pointer.h"
#include <memory>

namespace Yuggoth {

struct ModelComponent {
  IntrusivePointer<Model> model_;
};

struct BufferOwningModelComponent {
  IntrusivePointer<BufferOwningModel> model_;
};

} // namespace Yuggoth

#endif // YUGGOTH_MODEL_COMPONENT_H