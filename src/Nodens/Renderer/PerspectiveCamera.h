#pragma once

#include <glm/glm.hpp>

#include "Camera.h"

namespace Nodens {
class PerspectiveCamera : public Camera {
 public:
  PerspectiveCamera(float fov = 45, unsigned int width = 1280,
                    unsigned int height = 720, float nearPlane = 0.1f,
                    float farPlane = 100.0f);

  void SetFOV(const float fov) {
    m_FOV = fov;
    RecalculateProjectionMatrix();
  }
  void SetResolution(unsigned int width, unsigned int height) {
    m_Width = width;
    m_Hight = height;
    RecalculateProjectionMatrix();
  }

  const glm::vec3& GetPosition() const { return m_Position; }
  void SetPosition(const glm::vec3& position) {
    m_Position = position;
    RecalculateViewMatrix();
  }

  const glm::vec3& GetRotation() const { return m_Rotation; }
  void SetRotation(const glm::vec3& rotation) {
    m_Rotation = rotation;
    RecalculateViewMatrix();
  }

  const glm::vec3& GetUp() const { return m_Up; }
  const glm::vec3& GetFront() const { return m_Front; }
  const glm::vec3& GetRight() const { return m_Right; }

  void LookAt(const glm::vec3& target) {
    m_Target = target;
    RecalculateViewMatrix();
  }

  virtual const glm::mat4 GetProjectionMatrix() const override {
    return m_ProjectionMatrix;
  }
  virtual const glm::mat4 GetViewMatrix() const override {
    return m_ViewMatrix;
  }
  virtual const glm::mat4 GetViewProjectionMatrix() const override {
    return m_ViewProjectionMatrix;
  }
  bool m_UseTarget = false;

 private:
  void RecalculateProjectionMatrix();
  void RecalculateViewMatrix();

 private:
  glm::mat4 m_ProjectionMatrix;
  glm::mat4 m_ViewMatrix;
  glm::mat4 m_ViewProjectionMatrix;

  glm::vec3 m_Position = glm::vec3(0.0f);
  glm::vec3 m_Rotation = glm::vec3(0.0f, -90.0f, 0.0f);

  glm::vec3 m_Direction = glm::vec3(0.0f);
  glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 m_Right = glm::vec3(1.0f, 0.0f, 0.0f);

  glm::vec3 m_Target = m_Front;

  float m_FOV, m_Far, m_Near;
  unsigned int m_Width, m_Hight;
};

}  // namespace Nodens
