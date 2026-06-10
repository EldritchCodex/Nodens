export module Nodens.ImGuiLayer;

import Nodens.ImGuiRenderer;
import Nodens.Layer;
import Nodens.TimeStep;
import Nodens.DefaultTheme;
import Nodens.Events;
import std;

export namespace Nodens
{
class ImGuiLayer : public Layer
{
public:
    ImGuiLayer(const std::shared_ptr<ImGuiRenderer>& renderer, DefaultTheme theme);
    ~ImGuiLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnImGuiRender(TimeStep ts) override;
    void OnEvent(Event& e) override;

    void Begin();
    void End();

    void BlockEvents(bool block) { m_BlockEvents = block; }

private:
    bool                           m_BlockEvents = true;
    DefaultTheme                   m_Theme       = DefaultTheme::Dark;
    std::shared_ptr<ImGuiRenderer> m_Renderer;
};
} // namespace Nodens
