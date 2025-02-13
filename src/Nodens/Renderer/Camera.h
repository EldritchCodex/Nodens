#pragma once

#include <glm/glm.hpp>

namespace Nodens {
class Camera {
 public:
  virtual ~Camera() = default;

  virtual const glm::mat4 GetProjectionMatrix() const = 0;
  virtual const glm::mat4 GetViewMatrix() const = 0;
  virtual const glm::mat4 GetViewProjectionMatrix() const = 0;

 private:
  virtual void RecalculateViewMatrix() = 0;
};
}  // namespace Nodens
