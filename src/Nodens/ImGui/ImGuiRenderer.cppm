module;

#include <GLFW/glfw3.h>
#include <imgui.h>

export module Nodens.ImGuiRenderer;

export namespace Nodens
{
class ImGuiRenderer
{
public:
    virtual ~ImGuiRenderer() = default;

    virtual void Init(GLFWwindow* window)             = 0;
    virtual void Shutdown()                           = 0;
    virtual void NewFrame()                           = 0;
    virtual void RenderDrawData(ImDrawData* drawData) = 0;
};
} // namespace Nodens
