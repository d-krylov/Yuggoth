#include "model_component.h"

namespace Yuggoth {

ModelComponent::ModelComponent(const std::filesystem::path path) {
  model_ = std::make_shared<Model>(path);
}

} // namespace Yuggoth