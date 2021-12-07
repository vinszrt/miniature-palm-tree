#include "camera.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>

void Camera::computeProjectionMatrix(int width, int height) {
  const auto aspect{static_cast<float>(width) / static_cast<float>(height)};
  m_projMatrix = glm::perspective(glm::radians(70.0f), aspect, 0.01f, 10.0f);
}

void Camera::pan(float speed) {
  glm::mat4 transform{glm::mat4(1.0f)};

  // Rotate camera around its local y axis
  transform = glm::translate(transform, m_eye);
  transform = glm::rotate(transform, -speed, m_up);
  transform = glm::translate(transform, -m_eye);

  m_at = transform * glm::vec4(m_at, 1.0f);
}

void Camera::computeViewMatrix() {
  m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
}

void Camera::restart() {
  m_eye = glm::vec3(0.0f, 0.0f, 2.0f);  // Camera position
  m_at = glm::vec3(0.0f, 0.0f, 0.0f);   // Look-at point
  m_up = glm::vec3(0.0f, 1.0f, 0.0f);   // "up" direction
}