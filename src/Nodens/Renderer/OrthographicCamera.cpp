#include "OrthographicCamera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "ndpch.h"

namespace Nodens {
OrthographicCamera::OrthographicCamera(float left, float right, float bottom,
                                       float top)
    : m_Left(left),
      m_Right(right),
      m_Bottom(bottom),
      m_Top(top),
      m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
      m_ViewMatrix(1.0f),
      m_Zoom(1.0f) {
  m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void OrthographicCamera::RecalculateProjectionMatrix() {
  m_ProjectionMatrix =
      glm::ortho(m_Left * m_Zoom, m_Right * m_Zoom, m_Bottom * m_Zoom,
                 m_Top * m_Zoom, -1.0f, 1.0f);
  RecalculateViewMatrix();
}

void OrthographicCamera::RecalculateViewMatrix() {
  glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
                        glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation),
                                    glm::vec3(0, 0, 1));

  m_ViewMatrix = glm::inverse(transform);
  m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}
}  // namespace Nodens