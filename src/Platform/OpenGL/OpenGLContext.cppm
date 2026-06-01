module;

#include <GLFW/glfw3.h>

export module Nodens.OpenGLContext;

import Nodens.GraphicsContext;

export namespace Nodens
{
class OpenGLContext : public GraphicsContext
{
public:
    OpenGLContext(GLFWwindow* windowHandle);

    void Init() override;
    void SwapBuffers() override;

private:
    GLFWwindow* m_WindowHandle;
};
} // namespace Nodens
