#pragma once

#include <glm/glm.hpp>

namespace Nodens {
class Light {
 public:
  enum class Type { Point, Directional };

 public:
  Light() = default;
  virtual ~Light() = default;

  virtual const Type GetType() const = 0;
  virtual const glm::vec3 GetPosition() const = 0;
  virtual const glm::vec3 GetDirection() const = 0;
  virtual const glm::vec3 GetColor() const = 0;
  virtual void SetDirection(glm::vec3 direction) = 0;
  virtual void SetPosition(glm::vec3 position) = 0;
  virtual void SetColor(glm::vec3 color) = 0;
};
}  // namespace Nodens
