#pragma once

#include <fstream>
#include <list>
#include <numbers>

#include "nodens.h" // Nodens Engine lib

/// @brief Layer for the CircularWave3D example.
/// This layer demonstrates how to use ImPlot and ImPlot3D to visualize a 3D circular wave.
class CircularWave3DLayer : public Nodens::Layer
{
public:
    /// @brief Constructs the CircularWave3DLayer.
    CircularWave3DLayer();

    /// @brief Called every frame to update the layer.
    /// @param ts TimeStep object representing the time elapsed since the last frame.
    void OnUpdate(Nodens::TimeStep ts) override;

    /// @brief This function is called once per frame after OnUpdate.
    /// @param ts TimeStep object representing the time elapsed since the last frame.
    void OnImGuiRender(Nodens::TimeStep ts) override;

    /// @brief Called when an event is dispatched to the layer.
    /// @param event The event to be handled.
    void OnEvent(Nodens::Event& event) override;

    /// @brief Called when a window resize event occurs.
    /// @param event The window resize event.
    /// @return True if the event was handled, false otherwise.
    bool OnWindowResizeEvent(Nodens::WindowResizeEvent& event);

private:
    static constexpr size_t kNumberOfPoints = 1000;

    float kFrequency = 5.f * std::numbers::pi_v<float>;
    float t[kNumberOfPoints], xs[kNumberOfPoints], ys[kNumberOfPoints];
};
