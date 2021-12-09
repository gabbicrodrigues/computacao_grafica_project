#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtc/matrix_inverse.hpp>

void OpenGLWindow::handleEvent(SDL_Event& ev) {
  if (ev.type == SDL_KEYDOWN) {
    if (ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w)
      m_dollySpeed = 1.0f;
    if (ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s)
      m_dollySpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a)
      m_panSpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d)
      m_panSpeed = 1.0f;
    if (ev.key.keysym.sym == SDLK_q) m_truckSpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_e) m_truckSpeed = 1.0f;
  }
  if (ev.type == SDL_KEYUP) {
    if ((ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w) &&
        m_dollySpeed > 0)
      m_dollySpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s) &&
        m_dollySpeed < 0)
      m_dollySpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_q && m_truckSpeed < 0) m_truckSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_e && m_truckSpeed > 0) m_truckSpeed = 0.0f;
  }
}

void OpenGLWindow::initializeGL() {
  abcg::glClearColor(135.0f/255.0f, 206.0f/255.0f, 255.0f/235.0f, 0);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program = createProgramFromFile(getAssetsPath() + "shaders/texture.vert",
                                  getAssetsPath() + "shaders/texture.frag");

  // Load model
  m_cat.loadObj(getAssetsPath() + "cat.obj");
  m_cat.setupVAO(m_program);
  
  m_bunny.loadObj(getAssetsPath() + "bunny.obj");
  m_bunny.setupVAO(m_program);

  m_wolf.loadObj(getAssetsPath() + "Wolf.obj");
  m_wolf.setupVAO(m_program);

  m_floor.loadDiffuseTexture(getAssetsPath() + "maps/seamless_grass.jpg");
  m_floor.loadObj(getAssetsPath() + "grassPatch.obj");
  m_floor.setupVAO(m_program);

  m_Ka = m_floor.getKa();
  m_Kd = m_floor.getKd();
  m_Ks = m_floor.getKs();
  m_shininess = m_floor.getShininess();

  // m_tree.loadDiffuseTexture(getAssetsPath() + "maps/free grass.png");
  // m_tree.loadObj(getAssetsPath() + "free grass.obj");
  // m_tree.setupVAO(m_program);

  // m_Ka = m_tree.getKa();
  // m_Kd = m_tree.getKd();
  // m_Ks = m_tree.getKs();
  // m_shininessTree = m_tree.getShininess();

  resizeGL(getWindowSettings().width, getWindowSettings().height);
}

void OpenGLWindow::paintGL() {
  update();

  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  abcg::glUseProgram(m_program);

  // Get location of uniform variables (could be precomputed)
  const GLint viewMatrixLoc{
      abcg::glGetUniformLocation(m_program, "viewMatrix")};
  const GLint projMatrixLoc{
      abcg::glGetUniformLocation(m_program, "projMatrix")};
  const GLint modelMatrixLoc{
      abcg::glGetUniformLocation(m_program, "modelMatrix")};
  const GLint colorLoc{abcg::glGetUniformLocation(m_program, "color")};
  const GLint normalMatrixLoc{glGetUniformLocation(m_program, "normalMatrix")};
  const GLint lightDirLoc{glGetUniformLocation(m_program, "lightDirWorldSpace")};
  const GLint shininessLoc{glGetUniformLocation(m_program, "shininess")};
  const GLint IaLoc{glGetUniformLocation(m_program, "Ia")};
  const GLint IdLoc{glGetUniformLocation(m_program, "Id")};
  const GLint IsLoc{glGetUniformLocation(m_program, "Is")};
  const GLint KaLoc{glGetUniformLocation(m_program, "Ka")};
  const GLint KdLoc{glGetUniformLocation(m_program, "Kd")};
  const GLint KsLoc{glGetUniformLocation(m_program, "Ks")};
  const GLint diffuseTexLoc{glGetUniformLocation(m_program, "diffuseTex")};
  const GLint mappingModeLoc{glGetUniformLocation(m_program, "mappingMode")};

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_projMatrix[0][0]);

  abcg::glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);

  abcg::glUniform1i(diffuseTexLoc, 0);
  
  abcg::glUniform1i(mappingModeLoc, m_mappingMode);

  const auto lightDirRotated{m_camera.m_projMatrix * m_lightDir};
  abcg::glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);                         

  const auto modelViewMatrix{glm::mat3(m_viewMatrix * m_modelMatrix)};
  const glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);
  // abcg::glBindVertexArray(m_VAO);

  //Draw floor
  glm::mat4 floor{1.0f};
  floor = glm::translate(floor, glm::vec3(0.0f, -1.0f, 0.0f));
  // floor = glm::rotate(floor, glm::radians(90.0f), glm::vec3(1, 0, 0));
  floor = glm::scale(floor, glm::vec3(10.0f));

  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &floor[0][0]);

  glUniform1f(shininessLoc, m_shininess);
  glUniform4fv(KaLoc, 1, &m_Ka.x);
  glUniform4fv(KdLoc, 1, &m_Kd.x);
  glUniform4fv(KsLoc, 1, &m_Ks.x);
  m_floor.render();
  // m_model.render(m_trianglesToDraw);

  // Draw white cat
  glm::mat4 cat{1.0f};
  cat = glm::translate(cat, glm::vec3(-1.2f, -1.0f, 0.0f));
  cat = glm::rotate(cat, glm::radians(90.0f), glm::vec3(0, 1, 0));
  cat = glm::scale(cat, glm::vec3(0.8f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &cat[0][0]);
  abcg::glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
  m_cat.render();
  // abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
  //                      nullptr);

  // Draw yellow bunny
  glm::mat4 bunny{1.0f};
  bunny = glm::translate(bunny, glm::vec3(2.0f, -1.0f, 0.5f));
  bunny = glm::rotate(bunny, glm::radians(90.0f), glm::vec3(0, 1, 0));
  bunny = glm::scale(bunny, glm::vec3(0.5f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &bunny[0][0]);
  abcg::glUniform4f(colorLoc, 1.0f, 0.8f, 0.0f, 1.0f);
  m_bunny.render();
  // abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
  //                      nullptr);

  // Draw blue wolf
  glm::mat4 wolf{1.0f};
  wolf = glm::translate(wolf, glm::vec3(3.2f, -0.8f, 0.0f));
  wolf = glm::rotate(wolf, glm::radians(90.0f), glm::vec3(0, 1, 0));
  wolf = glm::scale(wolf, glm::vec3(1.2f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &wolf[0][0]);
  abcg::glUniform4f(colorLoc, 0.0f, 0.8f, 1.0f, 1.0f);
  m_wolf.render();

  // //Draw tree
  // glm::mat4 tree{1.0f};
  // tree = glm::translate(tree, glm::vec3(-2.5f, -0.8f, -1.75f));
  // tree = glm::scale(tree, glm::vec3(0.25f));

  // glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &tree[0][0]);
  // glUniform1f(shininessLoc, m_shininessTree);
  // glUniform4fv(KaLoc, 1, &m_KaTree.x);
  // glUniform4fv(KdLoc, 1, &m_KdTree.x);
  // glUniform4fv(KsLoc, 1, &m_KsTree.x);
  // m_tree.render();

  // abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void OpenGLWindow::paintUI() { 
  // abcg::OpenGLWindow::paintUI(); 
  ImGui::SetNextWindowPos(ImVec2(m_viewportWidth - 280, 0));
  ImGui::SetNextWindowSize(ImVec2(280, 85));
  auto windowFlags{ImGuiWindowFlags_NoBackground |
                  ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs};
  ImGui::Begin("score", nullptr, windowFlags);

  ImGui::Text("Movimente a câmera com as setas");
  ImGui::Text("e com as teclas Q, W, E, A, S, D");

  ImGui::End();

  // Create window for light sources
  const auto widgetSize{ImVec2(260, 230)};
  ImGui::SetNextWindowPos(ImVec2(m_viewportWidth - widgetSize.x - 5,
                                m_viewportHeight - widgetSize.y - 5));
  ImGui::SetNextWindowSize(widgetSize);
  ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoDecoration);

  ImGui::Text("Light properties");

  // Slider to control light properties
  ImGui::PushItemWidth(widgetSize.x - 36);
  ImGui::ColorEdit3("Ia", &m_Ia.x, ImGuiColorEditFlags_Float);
  ImGui::ColorEdit3("Id", &m_Id.x, ImGuiColorEditFlags_Float);
  ImGui::ColorEdit3("Is", &m_Is.x, ImGuiColorEditFlags_Float);
  ImGui::PopItemWidth();

  // Slider to control the specular shininess
  ImGui::PushItemWidth(widgetSize.x - 16);
  ImGui::SliderFloat("", &m_shininess, 0.0f, 500.0f, "shininess: %.1f");
  ImGui::PopItemWidth();

  ImGui::Spacing();

  ImGui::Text("Quais animais voce ve?");

  // which animals do you see? combo box
  static std::size_t currentIndex{};
  std::vector<std::string> comboItems{"bunny|elephant|wolf", "cat|bunny|wolf", "cat|bunny|dog"};

  ImGui::PushItemWidth(120);
  if (ImGui::BeginCombo(" ", comboItems.at(currentIndex).c_str())) {
    for (auto index : iter::range(comboItems.size())) {
      const bool isSelected{currentIndex == index};
      if (ImGui::Selectable(comboItems.at(index).c_str(), isSelected))
        currentIndex = index;
      if (isSelected) ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();
  }
  ImGui::PopItemWidth();

  if (currentIndex == 1) {
    ImGui::Text("Acertou!");
  } else {
    ImGui::Text("Errado, tente novamente");
    }

  ImGui::End();

  }

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_camera.computeProjectionMatrix(width, height);
}

void OpenGLWindow::terminateGL() {

  abcg::glDeleteProgram(m_program);
}

void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
}