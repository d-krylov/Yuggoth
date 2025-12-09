#ifndef YUGGOTH_MODEL_COMPONENT_H
#define YUGGOTH_MODEL_COMPONENT_H

#include "yuggoth/asset/include/model.h"
#include <memory>

namespace Yuggoth {

struct ModelComponent {

  ModelComponent(const std::filesystem::path path);

  std::shared_ptr<Model> model_;
};

} // namespace Yuggoth

#endif // YUGGOTH_MODEL_COMPONENT_H