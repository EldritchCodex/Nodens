#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "ndpch.h"

namespace Nodens {
OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
    : m_WindowHandle(windowHandle) {
  ND_CORE_ASSERT(windowHandle, "Window hadnle is null!")
}

void OpenGLContext::Init() {
  glfwMakeContextCurrent(m_WindowHandle);
  // glfwSetInputMode(m_WindowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Initialize GLAD
  int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  ND_CORE_ASSERT(status, "Failed to initialize glad!");

  ND_CORE_INFO("OpenGL Info:");
  ND_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
  ND_CORE_INFO("  GPU Used: {0}", glGetString(GL_RENDERER));

  glEnable(GL_MULTISAMPLE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void OpenGLContext::SwapBuffers() { glfwSwapBuffers(m_WindowHandle); }
}  // namespace Nodens