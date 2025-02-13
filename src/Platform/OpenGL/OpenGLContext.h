#pragma once

#include "Nodens/GraphicsContext.h"

struct GLFWwindow;

namespace Nodens {

class OpenGLContext : public GraphicsContext {
 public:
  OpenGLContext(GLFWwindow* windowHandle);

  virtual void Init() override;
  virtual void SwapBuffers() override;

 private:
  GLFWwindow* m_WindowHandle;
};
}  // namespace Nodens
