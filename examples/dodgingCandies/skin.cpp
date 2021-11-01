#include "skin.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Ship::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  m_rotation = 0.0f;
  m_translation = glm::vec2(0.0f, -0.75f);
  m_velocity = glm::vec2(0);

  // clang-format off
  std::array<glm::vec2, 25> positions{
      // Ship body
      glm::vec2{-0.5f, +4.0f}, glm::vec2{0.5f, +4.0f},
      glm::vec2{-01.25f, +3.0f}, glm::vec2{+01.25f, +3.0f},
      glm::vec2{-0.75f, +2.0f}, glm::vec2{+0.75f, +2.0f},
      glm::vec2{-1.5f, +1.5f}, glm::vec2{+1.5f, +1.5f},
      glm::vec2{-3.0f, -1.0f}, glm::vec2{+3.0f, -1.0f},
      glm::vec2{-1.25f, +0.25f}, glm::vec2{+1.25f, +0.25f},
      glm::vec2{-2.0f, -04.0f}, glm::vec2{+2.0f, -4.0f},
      glm::vec2{+0.0f, -2.0f},
      };

  // Normalize
  for (auto &position : positions) {
    position /= glm::vec2{4.5f, 4.5f};
  }

  const std::array indices{0, 1, 3,
                          0, 2, 3,

                          4, 5, 7,
                          4, 6, 7,
                          6, 7, 10,
                          6, 8, 10,
                          7, 10, 11,
                          7, 9, 11,
                          10, 11, 12,
                          10, 11, 13,
                          2, 3, 4,
                          3, 4, 5,};
  // clang-format on

  // Generate VBO
  abcg::glGenBuffers(1, &m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_ebo);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Ship::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_vao);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform1f(m_rotationLoc, m_rotation);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);


  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Ship::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteBuffers(1, &m_ebo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

void Ship::update(const GameData &gameData, float deltaTime) {
  
  if (gameData.m_state == State::Playing) {
    // Thrust in the forward vector
    glm::vec2 forward = glm::rotate(glm::vec2{0.0f, 0.1f}, m_rotation);
    m_velocity += forward * deltaTime;
  }

  // Walk left
  if (m_translation.x >= -0.90){
    if (gameData.m_input[static_cast<size_t>(Input::Left)] &&
        gameData.m_state == State::Playing) {
      m_translation.x -= 1.5 * deltaTime;
    }
  }

  //Walk right
  if (m_translation.x <= 0.90){
    if (gameData.m_input[static_cast<size_t>(Input::Right)] &&
        gameData.m_state == State::Playing) {
      m_translation.x += 1.5 * deltaTime;
    }
  }
}
