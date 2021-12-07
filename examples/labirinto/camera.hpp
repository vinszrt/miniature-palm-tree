#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <fmt/core.h>

#include <array>
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class OpenGLWindow;

class Camera {
 public:
  template <std::size_t SIZE>
  void dolly(float speed, std::array<glm::vec3, SIZE> m_wallPositions);

  template <std::size_t SIZE>
  void truck(float speed, std::array<glm::vec3, SIZE> m_wallPositions);

  void pan(float);

  void computeProjectionMatrix(int, int);
  void computeViewMatrix();
  void restart();

 private:
  friend OpenGLWindow;

  glm::vec3 m_eye;  // Camera position
  glm::vec3 m_at;   // Look-at point
  glm::vec3 m_up;   // "up" direction

  // Matrix to change from world space to camera space
  glm::mat4 m_viewMatrix{1.0f};

  // Matrix to change from camera space to clip space
  glm::mat4 m_projMatrix{1.0f};
};

template <std::size_t SIZE>
void Camera::dolly(float speed, std::array<glm::vec3, SIZE> m_wallPositions) {
  // Compute forward vector (view direction)
  const glm::vec3 forward{glm::normalize(m_at - m_eye)};

  const glm::vec3 m_next_eye =
      m_eye + forward * speed;  // Posição para onde a camera irá caso se mova
  const glm::vec2 proj_eye = {
      m_next_eye[0], m_next_eye[2]};  // Projeção da posição da camera no chão

  glm::vec2 proj_wall;  // Projeção da posição de um bloco de parede no chão
  int collision = 0;    // Identificador de colisão

  // Para cada parede, verifica se a nova posição da camera estará em colisão
  // com alguma parede
  for (long unsigned int i = 0; i < m_wallPositions.size(); i++) {
    proj_wall = {m_wallPositions.at(i)[0], m_wallPositions.at(i)[2]};

    if (glm::distance(proj_eye, proj_wall) < 0.81) {
      collision = 1;  // Colidiu
    }
  }

  // Verifica se a camera sairá do labirinto
  if (m_next_eye[0] > 9 || m_next_eye[0] < -9) collision = 1;

  // Move eye and center forward (speed > 0) or backward (speed < 0) se não
  // houve colisão
  if (collision == 0) {
    m_eye += forward * speed;
    m_at += forward * speed;
  }
}

template <std::size_t SIZE>
void Camera::truck(float speed, std::array<glm::vec3, SIZE> m_wallPositions) {
  // Compute forward vector (view direction)
  const glm::vec3 forward{glm::normalize(m_at - m_eye)};
  // Compute vector to the left
  const glm::vec3 left{glm::cross(m_up, forward)};

  const glm::vec3 m_next_eye =
      m_eye - left * speed;  // Posição para onde a camera irá caso se mova
  const glm::vec2 proj_eye = {
      m_next_eye[0], m_next_eye[2]};  // Projeção da posição da camera no chão

  glm::vec2 proj_wall;  // Projeção da posição do bloco de parede no chão
  int collision = 0;    // identificador de colisão

  // Para cada parede, verifica se a nova posição da camera estará em colisão
  // com alguma parede
  for (long unsigned int i = 0; i < m_wallPositions.size(); i++) {
    proj_wall = {m_wallPositions.at(i)[0], m_wallPositions.at(i)[2]};

    if (glm::distance(proj_eye, proj_wall) < 0.81) {
      collision = 1;  // Colidiu
    }
  }

  // Verifica se a camera sairá do labirinto
  if (m_next_eye[0] > 9 || m_next_eye[0] < -9) collision = 1;

  // Move eye and center to the left (speed < 0) or to the right (speed > 0) se
  // não houver colisão
  if (collision == 0) {
    m_eye -= left * speed;
    m_at -= left * speed;
  }
}

#endif