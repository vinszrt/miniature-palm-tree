#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include "abcg.hpp"
#include "abcg_elapsedtimer.hpp"
#include "camera.hpp"
#include "gamedata.hpp"
#include "imgui.h"
#include "model.hpp"
#include "trackball.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& ev) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;
  void makeMovement(float);
  void checkWinCondition();

 private:
  int m_viewportWidth{};
  int m_viewportHeight{};

  // Jogo
  GameData m_gameData;
  abcg::ElapsedTimer m_restartWaitTimer;
  ImFont* m_font{};

  // Camera
  Camera m_camera;
  float m_dollySpeed{0.0f};
  float m_truckSpeed{0.0f};
  float m_panSpeed{0.0f};

  // Modelo
  Model m_model;
  int m_trianglesToDraw{};
  Model m_bunny;
  int m_trianglesOfBunny{};
  Model m_ground;
  std::array<glm::mat4, 200> m_modelMatrix{glm::mat4(1.0f)};
  std::array<glm::vec3, 198> m_wallPositions;

  // Shaders
  std::vector<GLuint> m_programs;
  int m_currentProgramIndex{};

  // Mapping mode
  // 0: triplanar; 1: cylindrical; 2: spherical; 3: from mesh
  int m_mappingMode{};

  // Iluminação
  TrackBall m_trackBallLight;

  // Light and material properties
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{};
  glm::vec4 m_Kd{};
  glm::vec4 m_Ks{};
  float m_shininess{};

  void loadModel(std::string_view path);
  void loadBunny();
  void loadGround();
  void update();
  void loadMaze();
  void restart();
};

#endif