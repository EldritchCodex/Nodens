/// @file OpenGLContext.cppm
/// @brief OpenGL implementation of the GraphicsContext interface.
/// @ingroup Platform

module;

#include <GLFW/glfw3.h>

export module Nodens.OpenGLContext;

import Nodens.GraphicsContext;

export namespace Nodens
{
/// @brief Concrete GraphicsContext implementation for OpenGL via GLFW.
/// @details Creates an OpenGL rendering context on the provided GLFW window, loads
///          function pointers with GLAD, enables default GL state (multisample,
///          depth test, face culling, alpha blending), and handles buffer swapping.
/// @see GraphicsContext, GlfwWindow
/// @ingroup Platform
class OpenGLContext : public GraphicsContext
{
public:
    /// @brief Constructs the OpenGL context for a given GLFW window.
    /// @param windowHandle A valid GLFWwindow pointer. Must not be null.
    OpenGLContext(GLFWwindow* windowHandle);

    /// @brief Makes the context current, loads GL functions via GLAD, and sets default GL state.
    void Init() override;

    /// @brief Swaps the GLFW window's front and back buffers.
    void SwapBuffers() override;

private:
    GLFWwindow* m_WindowHandle; ///< The GLFW window this context is bound to.
};
} // namespace Nodens
