#include "camera.h"

namespace Yuggoth {

Camera::Camera(const Vector3f &position, const Vector3f &orientation) : position_(position), euler_(orientation) {
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

Matrix4f Camera::GetLookAt() const {
  return glm::lookAt(position_, position_ + front_, up_);
}

Matrix4f Camera::GetProjection() const {
  return glm::perspective(fov_, aspect_, near_, far_);
}

void Camera::UpdateCameraVectors() {
  auto base_front = Vector3f(0.0f, 0.0f, -1.0f);
  auto base_up = Vector3f(0.0f, 1.0f, 1.0f);
  up_ = glm::normalize(orientation_ * base_up);
  front_ = glm::normalize(orientation_ * base_front);
  right_ = glm::normalize(glm::cross(front_, up_));
  up_ = glm::normalize(glm::cross(right_, front_));
}

void Camera::SetOrientation(const Vector3f &orientation) {
  euler_ = orientation;
  orientation_ = Quaternion(orientation);
  UpdateCameraVectors();
}

Vector3f Camera::GetEulerAngles() const {
  return euler_;
}

} // namespace Yuggoth