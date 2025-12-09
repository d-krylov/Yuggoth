#include "yuggoth/mathematics/include/transform.h"

namespace Yuggoth {

Transform::Transform(const Vector3f &translation, const Vector3f &scale) : translation_(translation), scale_(scale) {
}

const Vector3f &Transform::GetTranslation() const {
  return translation_;
}

const Vector3f &Transform::GetScale() const {
  return scale_;
}

void Transform::SetTranslation(const Vector3f &translation) {
  translation_ = translation;
}

void Transform::SetScale(const Vector3f &scale) {
  scale_ = scale;
}

} // namespace Yuggoth