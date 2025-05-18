#include "Sinewave3dLayer.h"

#include <cmath>

Sinewave3dLayer::Sinewave3dLayer() : Layer("Sinewave3d") {
  ND_PROFILE_ZONE_SCOPED;

  // Set ImGui theme
  ImGui::StyleColorsDark();

  // Generate data for ImPlot
  for (int i = 0; i < 1001; i++) {
    xs[i] = i * 0.001f;
    ys[i] = std::sinf(10 * 3.1415f * xs[i]);
  }
  for (int i = 0; i < 1001; i++) {
    xs3d[i] = i * 0.001f;
    ys3d[i] = std::sinf(float(10 * 3.1415f * xs3d[i]));
    zs3d[i] = std::cosf(10 * 3.1415f * xs3d[i]);
  }
}  // TestLayer::TestLayer

void Sinewave3dLayer::OnUpdate(Nodens::TimeStep previous_update_duration) {
  ND_PROFILE_ZONE_SCOPED;

  for (int i = 0; i < 1001; i++) {
    xs[i] += 0.001f;
    ys[i] = std::sinf(10 * 3.1415f * xs[i]);

    xs3d[i] += 0.001f;
    ys3d[i] = std::sinf(10 * 3.1415f * xs3d[i]);
    zs3d[i] = std::cosf(10 * 3.1415f * xs3d[i]);
  }
}  // TestLayer::OnUpdate

void Sinewave3dLayer::OnImGuiRender(Nodens::TimeStep ts) {
  ND_PROFILE_ZONE_SCOPED;
  ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);

  // Implot example
  ImGui::Begin("ImPlot Example");
  if (ImPlot::BeginPlot("Sine Wave", ImVec2(-1, -1))) {
    ImPlot::SetupAxes("x", "y", ImPlotAxisFlags_AutoFit,
                      ImPlotAxisFlags_AutoFit);
    ImPlot::PlotLine("Sine Wave", xs, ys, 1001);
    ImPlot::EndPlot();
  }
  ImGui::End();

  // Implot3D example
  ImGui::Begin("ImPlot3D Example");
  if (ImPlot3D::BeginPlot("Sine Wave 3D", ImVec2(-1, -1))) {
    ImPlot3D::SetupAxes("x", "y", "z", ImPlot3DAxisFlags_AutoFit,
                        ImPlot3DAxisFlags_AutoFit, ImPlot3DAxisFlags_AutoFit);
    ImPlot3D::PlotLine("Sine Wave 3D", xs3d, ys3d, zs3d, 1001);
    ImPlot3D::EndPlot();
  }
  ImGui::End();
}  // TestLayer::OnImGuiRender

void Sinewave3dLayer::OnEvent(Nodens::Event& event) {
  ND_PROFILE_ZONE_SCOPED;

  Nodens::EventDispatcher dispatcher(event);
  dispatcher.Dispatch<Nodens::WindowResizeEvent>(
      ND_BIND_EVENT_FN(Sinewave3dLayer::OnWindowResizeEvent));
}  // TestLayer::OnEvent

bool Sinewave3dLayer::OnWindowResizeEvent(Nodens::WindowResizeEvent& event) {
  return true;
}  // TestLayer::OnMouseMovedEvent
