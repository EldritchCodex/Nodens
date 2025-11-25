#pragma once

#include <fstream>
#include <list>
#include <numbers>

#include "nodens.h" // Nodens Engine lib

class CircularWave3DLayer : public Nodens::Layer
{
public:
    /// \brief The constructor is called only once when the layer is created in
    /// the stack.
    CircularWave3DLayer();

    /// \brief This function is called once per frame.
    /// \param ts TimeStep object representing the time elapsed since the last frame.
    void OnUpdate(Nodens::TimeStep ts) override;

    /// \brief This function is called once per frame after OnUpdate.
    /// \param ts TimeStep object representing the time elapsed since the last frame.
    void OnImGuiRender(Nodens::TimeStep ts) override;

    void OnEvent(Nodens::Event& event) override;
    bool OnWindowResizeEvent(Nodens::WindowResizeEvent& event);

private:
    static constexpr size_t kNumberOfPoints = 1000;

    float kFrequency = 5.f * std::numbers::pi_v<float>;
    float t[kNumberOfPoints], xs[kNumberOfPoints], ys[kNumberOfPoints];
};
