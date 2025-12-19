#ifndef YUGGOTH_MODEL_COMPONENT_H
#define YUGGOTH_MODEL_COMPONENT_H

#include "yuggoth/asset/include/model.h"
#include "yuggoth/asset/include/buffer_owning_model.h"
#include <memory>

namespace Yuggoth {

struct ModelComponent {
  std::shared_ptr<Model> model_;
};

struct BufferOwningModelComponent {
  std::shared_ptr<BufferOwningModel> model_;
};

} // namespace Yuggoth

#endif // YUGGOTH_MODEL_COMPONENT_H