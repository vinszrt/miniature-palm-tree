#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "camera.hpp"

void OpenGLWindow::handleEvent(SDL_Event& event) {
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_dollySpeed = 1.5f;
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_dollySpeed = -1.5f;
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_panSpeed = -1.5f;
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_panSpeed = 1.5f;
    if (event.key.keysym.sym == SDLK_q) m_truckSpeed = -1.5f;
    if (event.key.keysym.sym == SDLK_e) m_truckSpeed = 1.5f;
  }
  if (event.type == SDL_KEYUP) {
    if ((event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) &&
        m_dollySpeed > 0)
      m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) &&
        m_dollySpeed < 0)
      m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_RIGHT ||
         event.key.keysym.sym == SDLK_d) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_q && m_truckSpeed < 0) m_truckSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_e && m_truckSpeed > 0) m_truckSpeed = 0.0f;
  }
}

void OpenGLWindow::loadMaze() {
  // Representa os blocos que compõe o labirinto e suas posições
  std::vector<glm::vec2> walls_positions = {
      {-9.0f, -18.0f}, {-9.0f, -17.0f}, {-9.0f, -16.0f}, {-9.0f, -15.0f},
      {-9.0f, -14.0f}, {-9.0f, -13.0f}, {-9.0f, -12.0f}, {-9.0f, -11.0f},
      {-9.0f, -10.0f}, {-9.0f, -9.0f},  {-9.0f, -8.0f},  {-9.0f, -7.0f},
      {-9.0f, -6.0f},  {-9.0f, -5.0f},  {-9.0f, -4.0f},  {-9.0f, -3.0f},
      {-9.0f, -2.0f},  {-9.0f, -1.0f},  {-9.0f, -0.0f},  {-8.0f, -18.0f},
      {-8.0f, -0.0f},  {-7.0f, -18.0f}, {-7.0f, -16.0f}, {-7.0f, -15.0f},
      {-7.0f, -14.0f}, {-7.0f, -12.0f}, {-7.0f, -11.0f}, {-7.0f, -10.0f},
      {-7.0f, -9.0f},  {-7.0f, -7.0f},  {-7.0f, -6.0f},  {-7.0f, -4.0f},
      {-7.0f, -3.0f},  {-7.0f, -2.0f},  {-7.0f, -0.0f},  {-6.0f, -18.0f},
      {-6.0f, -16.0f}, {-6.0f, -9.0f},  {-6.0f, -4.0f},  {-6.0f, -2.0f},
      {-6.0f, -0.0f},  {-5.0f, -18.0f}, {-5.0f, -16.0f}, {-5.0f, -14.0f},
      {-5.0f, -13.0f}, {-5.0f, -12.0f}, {-5.0f, -11.0f}, {-5.0f, -9.0f},
      {-5.0f, -8.0f},  {-5.0f, -7.0f},  {-5.0f, -6.0f},  {-5.0f, -4.0f},
      {-5.0f, -2.0f},  {-5.0f, -1.0f},  {-5.0f, -0.0f},  {-4.0f, -18.0f},
      {-4.0f, -16.0f}, {-4.0f, -14.0f}, {-4.0f, -12.0f}, {-4.0f, -11.0f},
      {-4.0f, -0.0f},  {-3.0f, -18.0f}, {-3.0f, -16.0f}, {-3.0f, -14.0f},
      {-3.0f, -11.0f}, {-3.0f, -9.0f},  {-3.0f, -7.0f},  {-3.0f, -6.0f},
      {-3.0f, -5.0f},  {-3.0f, -4.0f},  {-3.0f, -3.0f},  {-3.0f, -2.0f},
      {-3.0f, -0.0f},  {-2.0f, -18.0f}, {-2.0f, -14.0f}, {-2.0f, -12.0f},
      {-2.0f, -11.0f}, {-2.0f, -9.0f},  {-2.0f, -7.0f},  {-2.0f, -2.0f},
      {-2.0f, -1.0f},  {-2.0f, -0.0f},  {-1.0f, -18.0f}, {-1.0f, -17.0f},
      {-1.0f, -16.0f}, {-1.0f, -11.0f}, {-1.0f, -9.0f},  {-1.0f, -7.0f},
      {-1.0f, -5.0f},  {-1.0f, -4.0f},  {-1.0f, -2.0f},  {-1.0f, -1.0f},
      {-1.0f, -0.0f},  {0.0f, -14.0f},  {0.0f, -13.0f},  {0.0f, -11.0f},
      {0.0f, -9.0f},   {0.0f, -7.0f},   {0.0f, -5.0f},   {1.0f, -18.0f},
      {1.0f, -17.0f},  {1.0f, -15.0f},  {1.0f, -14.0f},  {1.0f, -13.0f},
      {1.0f, -12.0f},  {1.0f, -11.0f},  {1.0f, -9.0f},   {1.0f, -7.0f},
      {1.0f, -5.0f},   {1.0f, -3.0f},   {1.0f, -2.0f},   {1.0f, -1.0f},
      {1.0f, -0.0f},   {2.0f, -18.0f},  {2.0f, -17.0f},  {2.0f, -15.0f},
      {2.0f, -13.0f},  {2.0f, -9.0f},   {2.0f, -7.0f},   {2.0f, -5.0f},
      {2.0f, -0.0f},   {3.0f, -18.0f},  {3.0f, -13.0f},  {3.0f, -11.0f},
      {3.0f, -10.0f},  {3.0f, -9.0f},   {3.0f, -7.0f},   {3.0f, -5.0f},
      {3.0f, -4.0f},   {3.0f, -2.0f},   {3.0f, -0.0f},   {4.0f, -18.0f},
      {4.0f, -16.0f},  {4.0f, -14.0f},  {4.0f, -13.0f},  {4.0f, -11.0f},
      {4.0f, -5.0f},   {4.0f, -4.0f},   {4.0f, -2.0f},   {4.0f, -0.0f},
      {5.0f, -18.0f},  {5.0f, -16.0f},  {5.0f, -14.0f},  {5.0f, -13.0f},
      {5.0f, -11.0f},  {5.0f, -9.0f},   {5.0f, -8.0f},   {5.0f, -7.0f},
      {5.0f, -5.0f},   {5.0f, -2.0f},   {5.0f, -0.0f},   {6.0f, -18.0f},
      {6.0f, -17.0f},  {6.0f, -16.0f},  {6.0f, -14.0f},  {6.0f, -13.0f},
      {6.0f, -5.0f},   {6.0f, -3.0f},   {6.0f, -2.0f},   {6.0f, -0.0f},
      {7.0f, -18.0f},  {7.0f, -13.0f},  {7.0f, -12.0f},  {7.0f, -11.0f},
      {7.0f, -9.0f},   {7.0f, -8.0f},   {7.0f, -7.0f},   {7.0f, -6.0f},
      {7.0f, -5.0f},   {7.0f, -3.0f},   {7.0f, -2.0f},   {7.0f, -0.0f},
      {8.0f, -18.0f},  {8.0f, -16.0f},  {8.0f, -15.0f},  {8.0f, -9.0f},
      {8.0f, -8.0f},   {8.0f, -3.0f},   {8.0f, -0.0f},   {9.0f, -18.0f},
      {9.0f, -17.0f},  {9.0f, -16.0f},  {9.0f, -15.0f},  {9.0f, -14.0f},
      {9.0f, -13.0f},  {9.0f, -12.0f},  {9.0f, -11.0f},  {9.0f, -10.0f},
      {9.0f, -9.0f},   {9.0f, -8.0f},   {9.0f, -7.0f},   {9.0f, -6.0f},
      {9.0f, -5.0f},   {9.0f, -4.0f},   {9.0f, -3.0f},   {9.0f, -2.0f},
      {9.0f, -1.0f},   {9.0f, -0.0f}};

  auto* position = &(m_wallPositions.at(0));

  // Insere os blocos nas posições
  for (unsigned long i = 0; i < walls_positions.size(); i++) {
    position = &(m_wallPositions.at(i));
    *position =
        glm::vec3(walls_positions.at(i)[0], 0.0f, walls_positions.at(i)[1]);
  }
}

void OpenGLWindow::initializeGL() {
  // Load a new font
  ImGuiIO& io{ImGui::GetIO()};
  auto filename{getAssetsPath() + "Inconsolata-Medium.ttf"};
  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }

  abcg::glClearColor(0, 0, 0, 1);
  abcg::glEnable(GL_DEPTH_TEST);

  // Create programs
  const auto path{getAssetsPath() + "shaders/normalmapping"};
  const auto program{createProgramFromFile(path + ".vert", path + ".frag")};
  m_programs.push_back(program);

  restart();

  // Load default model
  loadModel(getAssetsPath() + "chamferbox.obj");
  loadBunny();
  loadGround();
  m_mappingMode = 0;  // "From mesh" option

  // Initial trackball spin
}

void OpenGLWindow::loadBunny() {
  m_bunny.terminateGL();

  m_bunny.loadDiffuseTexture(getAssetsPath() + "maps/bunny.jpg");
  m_bunny.loadObj(getAssetsPath() + "bunny.obj");
  m_bunny.setupVAO(m_programs.at(m_currentProgramIndex));
  m_trianglesOfBunny = m_bunny.getNumTriangles();
}

void OpenGLWindow::loadGround() {
  m_ground.terminateGL();

  m_ground.loadDiffuseTexture(getAssetsPath() + "maps/cement.jpg");
  m_ground.loadObj(getAssetsPath() + "chamferbox.obj");
  m_ground.setupVAO(m_programs.at(m_currentProgramIndex));
}

void OpenGLWindow::loadModel(std::string_view path) {
  m_model.terminateGL();

  m_model.loadDiffuseTexture(getAssetsPath() + "maps/brick_base.jpg");
  m_model.loadNormalTexture(getAssetsPath() + "maps/brick_normal.jpg");
  m_model.loadObj(path);
  m_model.setupVAO(m_programs.at(m_currentProgramIndex));
  m_trianglesToDraw = m_model.getNumTriangles();

  // Use material properties from the loaded model
  m_Ka = m_model.getKa();
  m_Kd = m_model.getKd();
  m_Ks = m_model.getKs();
  m_shininess = m_model.getShininess();

  loadMaze();
}

void OpenGLWindow::paintGL() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  // Use currently selected program
  const auto program{m_programs.at(m_currentProgramIndex)};
  abcg::glUseProgram(program);

  // Get location of uniform variables
  const GLint viewMatrixLoc{abcg::glGetUniformLocation(program, "viewMatrix")};
  const GLint projMatrixLoc{abcg::glGetUniformLocation(program, "projMatrix")};
  const GLint modelMatrixLoc{
      abcg::glGetUniformLocation(program, "modelMatrix")};
  const GLint normalMatrixLoc{
      abcg::glGetUniformLocation(program, "normalMatrix")};
  const GLint lightDirLoc{
      abcg::glGetUniformLocation(program, "lightDirWorldSpace")};
  const GLint shininessLoc{abcg::glGetUniformLocation(program, "shininess")};
  const GLint IaLoc{abcg::glGetUniformLocation(program, "Ia")};
  const GLint IdLoc{abcg::glGetUniformLocation(program, "Id")};
  const GLint IsLoc{abcg::glGetUniformLocation(program, "Is")};
  const GLint KaLoc{abcg::glGetUniformLocation(program, "Ka")};
  const GLint KdLoc{abcg::glGetUniformLocation(program, "Kd")};
  const GLint KsLoc{abcg::glGetUniformLocation(program, "Ks")};
  const GLint diffuseTexLoc{abcg::glGetUniformLocation(program, "diffuseTex")};
  const GLint normalTexLoc{abcg::glGetUniformLocation(program, "normalTex")};
  const GLint mappingModeLoc{
      abcg::glGetUniformLocation(program, "mappingMode")};

  // Set uniform variables used by every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_projMatrix[0][0]);
  abcg::glUniform1i(diffuseTexLoc, 0);
  abcg::glUniform1i(normalTexLoc, 1);
  abcg::glUniform1i(mappingModeLoc, m_mappingMode);

  const auto lightDirRotated{m_trackBallLight.getRotation() * m_lightDir};
  abcg::glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);

  for (const auto i : iter::range(m_wallPositions.size())) {
    const auto& position{m_wallPositions.at(i)};

    m_modelMatrix.at(i) = glm::translate(m_modelMatrix.at(i), position);

    // Set uniform variables of the current object
    abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE,
                             &m_modelMatrix.at(i)[0][0]);

    const auto modelViewMatrix{
        glm::mat3(m_camera.m_viewMatrix * m_modelMatrix.at(i))};
    glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
    abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

    abcg::glUniform1f(shininessLoc, m_shininess);
    abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
    abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
    abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);

    m_model.render(m_trianglesToDraw);
  }

  // BUNNY
  {
    m_modelMatrix.at(198) =
        glm::translate(m_modelMatrix.at(198), {0.0f, 0.0f, -20.0f});
    abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE,
                             &m_modelMatrix.at(198)[0][0]);

    const auto modelViewMatrix{
        glm::mat3(m_camera.m_viewMatrix * m_modelMatrix.at(198))};
    glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
    abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

    abcg::glUniform1f(shininessLoc, m_shininess);
    abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
    abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
    abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);

    m_bunny.render(m_trianglesOfBunny);
  }

  // GROUND
  {
    m_modelMatrix.at(199) =
        glm::translate(m_modelMatrix.at(199), {0.0f, -1.0f, -10.0f});
    m_modelMatrix.at(199) =
        glm::scale(m_modelMatrix.at(199), {20.0f, 0.5f, 20.0f});
    abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE,
                             &m_modelMatrix.at(199)[0][0]);

    const auto modelViewMatrix{
        glm::mat3(m_camera.m_viewMatrix * m_modelMatrix.at(199))};
    glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
    abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

    abcg::glUniform1f(shininessLoc, m_shininess);
    abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
    abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
    abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);

    m_ground.render(m_trianglesToDraw);
  }

  abcg::glUseProgram(0);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  // Create main window widget
  {
    const auto size{ImVec2(300, 85)};
    const auto position{ImVec2(((float)m_viewportWidth - size.x) / 2.0f,
                               ((float)m_viewportHeight - size.y) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);

    // Se jogador chegou ao fim do labirinto, exibe *You Win!*
    if (m_gameData.m_state == State::Win) {
      ImGui::Text("*You Win!*");
    }

    ImGui::PopFont();
    ImGui::End();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_camera.computeProjectionMatrix(width, height);
  m_trackBallLight.resizeViewport(width, height);
}

void OpenGLWindow::terminateGL() {
  m_model.terminateGL();
  for (const auto& program : m_programs) {
    abcg::glDeleteProgram(program);
  }
}

void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  // Wait 5 seconds before restarting
  if (m_gameData.m_state == State::Win && m_restartWaitTimer.elapsed() > 5) {
    restart();
    return;
  }

  for (const auto i : iter::range(m_wallPositions.size() + 2)) {
    m_modelMatrix.at(i) = glm::mat4(1.0f);
  }

  if (m_gameData.m_state == State::Playing) {
    makeMovement(deltaTime);
    checkWinCondition();
  }
}

void OpenGLWindow::makeMovement(float deltaTime) {
  m_camera.dolly(m_dollySpeed * deltaTime, m_wallPositions);
  m_camera.truck(m_truckSpeed * deltaTime, m_wallPositions);
  m_camera.pan(m_panSpeed * deltaTime);
  m_camera.computeViewMatrix();
}

// Verifica se o jogador chegou ao fim do labirinto
void OpenGLWindow::checkWinCondition() {
  if (m_camera.m_eye[2] < -19.0f) {
    m_gameData.m_state = State::Win;
    m_restartWaitTimer.restart();
  }
}

void OpenGLWindow::restart() {
  m_gameData.m_state = State::Playing;

  m_camera.restart();
}