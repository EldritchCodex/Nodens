#pragma once

#include <glm/glm.hpp>

#include "Camera.h"

namespace Nodens {
class OrthographicCamera : public Camera {
 public:
  OrthographicCamera(float left, float right, float bottom, float top);

  const glm::vec3& GetPosition() const { return m_Position; }
  void SetPosition(const glm::vec3& position) {
    m_Position = position;
    RecalculateViewMatrix();
  }

  const float GetRotation() const { return m_Rotation; }
  void SetRotation(const float rotation) {
    m_Rotation = rotation;
    RecalculateViewMatrix();
  }

  float GetZoom() { return m_Zoom; }
  void SetZoom(float zoom) {
    m_Zoom = zoom;
    RecalculateProjectionMatrix();
  }

  float GetWidth() { return m_Right - m_Left; }
  float GetHeight() { return m_Top - m_Bottom; }

  float GetBottom() { return m_Bottom; }
  float GetTop() { return m_Top; }
  float GetLeft() { return m_Left; }
  float GetRight() { return m_Right; }
  void SetSides(float left, float right, float bottom, float top) {
    m_Left = left;
    m_Right = right;
    m_Bottom = bottom;
    m_Top = top;
    RecalculateProjectionMatrix();
  };

  virtual const glm::mat4 GetProjectionMatrix() const override {
    return m_ProjectionMatrix;
  }
  virtual const glm::mat4 GetViewMatrix() const override {
    return m_ViewMatrix;
  }
  virtual const glm::mat4 GetViewProjectionMatrix() const override {
    return m_ViewProjectionMatrix;
  }

 private:
  void RecalculateViewMatrix();
  void RecalculateProjectionMatrix();

 private:
  glm::mat4 m_ProjectionMatrix;
  glm::mat4 m_ViewMatrix;
  glm::mat4 m_ViewProjectionMatrix;

  float m_Left;
  float m_Right;
  float m_Bottom;
  float m_Top;
  float m_Zoom;

  glm::vec3 m_Position = glm::vec3(0);
  float m_Rotation = 0;
};

}  // namespace Nodens
