#pragma once

#include <string>

namespace Nodens {
class Shader {
 public:
  virtual ~Shader() = default;

  virtual void Use() const = 0;
  virtual void Unbind() const = 0;

  static Shader* Create(const std::string& vertexSrc,
                        const std::string& fragmentSrc);
};
}  // namespace Nodens