#pragma once

#include "ndpch.h"
#include <imgui.h>

struct GLFWwindow;

namespace Nodens {

    class ImGuiRenderer
    {
    public:
        virtual ~ImGuiRenderer() = default;

        // Called during OnAttach. Takes the window to set up platform bindings.
        virtual void Init(GLFWwindow* window) = 0;

        // Called during OnDetach.
        virtual void Shutdown() = 0;

        // Called at the start of the frame (before ImGui::NewFrame)
        virtual void NewFrame() = 0;

        // Called at the end of the frame to render draw data
        virtual void RenderDrawData(ImDrawData* drawData) = 0;
    };

}