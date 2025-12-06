#pragma once

#include "Nodens/Events/ApplicationEvent.h"
#include "Nodens/Events/KeyEvent.h"
#include "Nodens/Events/MouseEvent.h"
#include "Nodens/Layer.h"

#include "Nodens/imgui/ImGuiRenderer.h" // Include the interface

namespace Nodens
{

class ImGuiLayer : public Layer
{
public:
    // Inject the renderer via constructor
    ImGuiLayer(const std::shared_ptr<ImGuiRenderer>& renderer);
    ~ImGuiLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnImGuiRender(TimeStep ts) override;

    void Begin();
    void End();

    void BlockEvents(bool block) { m_BlockEvents = block; }

    void SetDarkThemeColors();

private:
    bool                           m_BlockEvents = true;
    std::shared_ptr<ImGuiRenderer> m_Renderer; // Polymorphic renderer
};

} // namespace Nodens