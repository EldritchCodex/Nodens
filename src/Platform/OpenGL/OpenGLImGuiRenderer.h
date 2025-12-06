#pragma once

#include "Nodens/imgui/ImGuiRenderer.h"

namespace Nodens {

    class OpenGLImGuiRenderer : public ImGuiRenderer
    {
    public:
        OpenGLImGuiRenderer() = default;
        virtual ~OpenGLImGuiRenderer() = default;

        void Init(GLFWwindow* window) override;
        void Shutdown() override;
        void NewFrame() override;
        void RenderDrawData(ImDrawData* drawData) override;
    };

}