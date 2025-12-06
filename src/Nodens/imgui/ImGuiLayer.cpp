#include "ImGuiLayer.h"
#include "ndpch.h"

#include "Nodens/Application.h"
#include "Nodens/Profiling.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <implot.h>
#include <implot3d.h>

namespace Nodens
{

ImGuiLayer::ImGuiLayer(const std::shared_ptr<ImGuiRenderer>& renderer) : Layer("ImGuiLayer"), m_Renderer(renderer) {}

ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::OnAttach()
{
    ND_PROFILE_ZONE_SCOPED;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImPlot3D::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiDockNodeFlags_PassthruCentralNode;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding              = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    Application& app = Application::Get();
    // We assume the native window is always GLFW for now, but we cast safely
    GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

    // Use the injected renderer to initialize
    if (m_Renderer)
        m_Renderer->Init(window);
}

void ImGuiLayer::OnDetach()
{
    ND_PROFILE_ZONE_SCOPED;

    if (m_Renderer)
        m_Renderer->Shutdown();

    ImPlot3D::DestroyContext();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

void ImGuiLayer::Begin()
{
    ND_PROFILE_ZONE_SCOPED;

    // Delegate NewFrame to the renderer (handles backend specific updates)
    if (m_Renderer)
        m_Renderer->NewFrame();

    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);
}

void ImGuiLayer::End()
{
    ND_PROFILE_ZONE_SCOPED;

    ImGuiIO&     io  = ImGui::GetIO();
    Application& app = Application::Get();
    io.DisplaySize   = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

    // Rendering
    ImGui::Render();

    if (m_Renderer)
        m_Renderer->RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        // Note: UpdatePlatformWindows is generic ImGui logic
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void ImGuiLayer::OnImGuiRender(TimeStep ts) {}

} // namespace Nodens