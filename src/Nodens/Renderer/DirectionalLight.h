#pragma once

#include "Light.h"

namespace Nodens {
class DirectionalLight : public Light {
 public:
  DirectionalLight(glm::vec3 direction = glm::vec3(0.0f),
                   glm::vec4 lightColor = glm::vec4(1.0f))
      : m_Type(Type::Directional),
        m_Direction(direction),
        m_LightColor(lightColor) {}

  virtual inline const Type GetType() const override { return m_Type; }
  virtual inline const glm::vec3 GetPosition() const override {
    return glm::vec3(0);
  }
  virtual inline const glm::vec3 GetDirection() const override {
    return m_Direction;
  }
  virtual inline const glm::vec3 GetColor() const override {
    return m_LightColor;
  }
  virtual inline void SetDirection(glm::vec3 direction) override {
    m_Direction = direction;
  }
  virtual inline void SetPosition(glm::vec3 position) override {}
  virtual inline void SetColor(glm::vec3 color) override {
    m_LightColor = color;
  }

 private:
  Type m_Type;
  glm::vec3 m_Direction;
  glm::vec3 m_LightColor;
};

}  // namespace Nodens