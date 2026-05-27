#pragma once

#include "AsyncEvent.h"
#include "nodens.h"
#include <Nodens/Events/AsyncEventBus.h>
#include <implot.h>

#include <mutex>
#include <sstream>
#include <vector>

/// @brief Layer for the AsyncEvent example.
/// This layer demonstrates how to use the Nodens::AsyncEventBus to dispatch events that are processed in background
/// threads.
class AsyncEventLayer : public Nodens::Layer
{
public:
    /// @brief Constructs the AsyncEventLayer.
    AsyncEventLayer();

    /// @brief Destroys the AsyncEventLayer.
    virtual ~AsyncEventLayer() = default;

    /// @brief Called when the layer is attached to the layer stack.
    virtual void OnAttach() override;

    /// @brief Called every frame to update the layer.
    /// @param ts The time since the last frame.
    virtual void OnUpdate(Nodens::TimeStep ts) override;

    /// @brief Called every frame to render the ImGui UI.
    /// @param ts The time since the last frame.
    virtual void OnImGuiRender(Nodens::TimeStep ts) override;

private:
    /// @brief Adds a scan result to the data vectors.
    /// This function is thread-safe.
    /// @param e The event containing the scan results.
    void AddResult(const PlanetaryScanEvent& e);

private:
    // Simulation State
    float m_TimePassed  = 0.0f;
    int   m_ScanCounter = 0;

    // ==============================================================
    // VISUALIZATION DATA
    // ==============================================================
    std::mutex m_DataMutex;

    // Plot 1: Galaxy Composition (Scatter Plot)
    std::vector<float> m_GalaxyDistances; // X-Axis
    std::vector<float> m_GalaxyDensities; // Y-Axis

    // Plot 2: Performance Stats (Line/Histogram)
    std::vector<float> m_LatencyHistory;
    float              m_MaxLatency = 0.0f;
};