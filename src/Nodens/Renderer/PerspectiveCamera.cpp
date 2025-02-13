#include "PerspectiveCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ndpch.h"

namespace Nodens {
PerspectiveCamera::PerspectiveCamera(float fov, unsigned int width,
                                     unsigned int height, float nearPlane,
                                     float farPlane)
    : m_FOV(fov),
      m_Width(width),
      m_Hight(height),
      m_Far(farPlane),
      m_Near(nearPlane)

{
  m_ProjectionMatrix = glm::perspective(
      glm::radians(fov), (float)width / height, nearPlane, farPlane);
  m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
  m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void PerspectiveCamera::RecalculateProjectionMatrix() {
  m_ProjectionMatrix = glm::perspective(
      glm::radians(m_FOV), (float)m_Width / m_Hight, m_Near, m_Far);
  m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void PerspectiveCamera::RecalculateViewMatrix() {
  if (m_UseTarget) {
    m_ViewMatrix = glm::lookAt(m_Position, m_Target, m_Up);
  } else {
    m_Direction.x =
        cos(glm::radians(m_Rotation.y)) * cos(glm::radians(m_Rotation.x));
    m_Direction.y = sin(glm::radians(m_Rotation.x));
    m_Direction.z =
        sin(glm::radians(m_Rotation.y)) * cos(glm::radians(m_Rotation.x));
    m_Front = glm::normalize(m_Direction);
    m_Right = glm::normalize(glm::cross(m_Front, m_Up));
    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
  }

  m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}
}  // namespace Nodens