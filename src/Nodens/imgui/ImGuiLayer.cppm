export module Nodens.ImGuiLayer;

import Nodens.ImGuiRenderer;
import Nodens.Layer;
import Nodens.TimeStep;
import std;

export namespace Nodens
{
class ImGuiLayer : public Layer
{
public:
    ImGuiLayer(const std::shared_ptr<ImGuiRenderer>& renderer);
    ~ImGuiLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnImGuiRender(TimeStep ts) override;

    void Begin();
    void End();

    void BlockEvents(bool block) { m_BlockEvents = block; }

    void SetDarkThemeColors();

private:
    bool                           m_BlockEvents = true;
    std::shared_ptr<ImGuiRenderer> m_Renderer;
};
} // namespace Nodens
