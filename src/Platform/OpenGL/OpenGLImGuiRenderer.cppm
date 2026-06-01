module;

#include <GLFW/glfw3.h>
#include <imgui.h>

export module Nodens.OpenGLImGuiRenderer;

import Nodens.ImGuiRenderer;

export namespace Nodens
{
class OpenGLImGuiRenderer : public ImGuiRenderer
{
public:
    OpenGLImGuiRenderer() = default;
    ~OpenGLImGuiRenderer() override = default;

    void Init(GLFWwindow* window) override;
    void Shutdown() override;
    void NewFrame() override;
    void RenderDrawData(ImDrawData* drawData) override;
};
} // namespace Nodens
