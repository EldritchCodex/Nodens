#include "CircularWave3DLayer.h"

#include <cmath>

CircularWave3DLayer::CircularWave3DLayer() : Layer("CircularWave3D")
{
    ND_PROFILE_ZONE_SCOPED;

    // Set ImGui theme
    ImGui::StyleColorsDark();

    constexpr float kIncrement = 0.001f;

    // Generate data for ImPlot
    for (int i = 0; i < kNumberOfPoints; i++)
    {
        t[i]  = i * kIncrement;
        xs[i] = std::sin(kFrequency * t[i]);
        ys[i] = std::cos(kFrequency * t[i]);
    }
} // CircularWave3DLayer::CircularWave3DLayer

void CircularWave3DLayer::OnUpdate(Nodens::TimeStep previous_update_duration)
{
    ND_PROFILE_ZONE_SCOPED;

    constexpr float kIncrement = 0.001f;
    for (int i = 0; i < kNumberOfPoints; i++)
    {
        t[i] += kIncrement;
        xs[i] = std::sin(kFrequency * t[i]);
        ys[i] = std::cos(kFrequency * t[i]);
    }
} // CircularWave3DLayer::OnUpdate

void CircularWave3DLayer::OnImGuiRender(Nodens::TimeStep ts)
{
    ND_PROFILE_ZONE_SCOPED;

    // Implot
    ImGui::Begin("ImPlot Example");
    if (ImPlot::BeginPlot("Wave components", ImVec2(-1, -1)))
    {
        ImPlot::SetupAxes("t", "x-y", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
        ImPlot::PlotLine("Sine", t, xs, kNumberOfPoints);
        ImPlot::PlotLine("Cosine", t, ys, kNumberOfPoints);
        ImPlot::EndPlot();
    }
    ImGui::End();

    // Implot3D
    ImGui::Begin("ImPlot3D Example");
    if (ImPlot3D::BeginPlot("3D Circular Wave", ImVec2(-1, -1)))
    {
        ImPlot3D::SetupAxes(
            "t", "x", "y", ImPlot3DAxisFlags_AutoFit, ImPlot3DAxisFlags_AutoFit, ImPlot3DAxisFlags_AutoFit);
        ImPlot3D::PlotLine("Wave", t, xs, ys, kNumberOfPoints);
        ImPlot3D::EndPlot();
    }
    ImGui::End();
} // CircularWave3DLayer::OnImGuiRender

void CircularWave3DLayer::OnEvent(Nodens::Event& event)
{
    ND_PROFILE_ZONE_SCOPED;

    Nodens::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Nodens::WindowResizeEvent>(ND_BIND_EVENT_FN(CircularWave3DLayer::OnWindowResizeEvent));
} // CircularWave3DLayer::OnEvent

bool CircularWave3DLayer::OnWindowResizeEvent(Nodens::WindowResizeEvent& event)
{
    return true;
} // CircularWave3DLayer::OnMouseMovedEvent
