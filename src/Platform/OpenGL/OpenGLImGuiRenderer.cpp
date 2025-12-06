#include "ndpch.h"
#include "OpenGLImGuiRenderer.h"

#include <imgui.h>
// These headers are now isolated to this file
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Nodens {

    void OpenGLImGuiRenderer::Init(GLFWwindow* window)
    {
        // Setup Platform/Renderer bindings
        // Note: InitForOpenGL is specific to this implementation
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 450");
    }

    void OpenGLImGuiRenderer::Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
    }

    void OpenGLImGuiRenderer::NewFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
    }

    void OpenGLImGuiRenderer::RenderDrawData(ImDrawData* drawData)
    {
        ImGui_ImplOpenGL3_RenderDrawData(drawData);
    }

}