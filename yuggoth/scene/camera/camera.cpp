#include "camera.h"

namespace Yuggoth {

Camera::Camera() {
}

void Camera::SetPosition(const Vector3f &position) {
  position_ = position;
}

const Vector3f &Camera::GetPosition() const {
  return position_;
}

void Camera::SetAspect(float aspect) {
  aspect_ = aspect;
}

float Camera::GetAspect() const {
  return aspect_;
}

void Camera::SetFOV(float fov) {
  fov_ = fov;
}

float Camera::GetFOV() const {
  return fov_;
}

} // namespace Yuggoth