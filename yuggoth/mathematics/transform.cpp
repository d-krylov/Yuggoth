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

const Vector3f &Transform::GetRotation() const {
  return euler_;
}

void Transform::SetOrientation(const Vector3f &orientation) {
  euler_ = orientation;
  orientation_ = Quaternion(orientation);
}

void Transform::SetScale(const Vector3f &scale) {
  scale_ = scale;
}

Matrix4f Transform::GetMatrix() const {
  auto translate = glm::translate(Matrix4f(1.0f), translation_);
  auto rotate = glm::mat4_cast(orientation_);
  auto scale = glm::scale(Matrix4f(1.0f), scale_);
  return translate * rotate * scale;
}

} // namespace Yuggoth