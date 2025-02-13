#pragma once

#include "Nodens/Core.h"

namespace Nodens {

class GraphicsContext {
 public:
  virtual void Init() = 0;
  virtual void SwapBuffers() = 0;
};

}  // namespace Nodens