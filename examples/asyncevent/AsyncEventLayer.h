#pragma once

#include "AsyncEvent.h"
#include <Nodens.h>
#include <Nodens/Events/AsyncEventBus.h>
#include <implot.h>

#include <mutex>
#include <sstream>
#include <vector>

class AsyncEventLayer : public Nodens::Layer
{
public:
    AsyncEventLayer();
    virtual ~AsyncEventLayer() = default;

    virtual void OnAttach() override;
    virtual void OnUpdate(Nodens::TimeStep ts) override;
    virtual void OnImGuiRender(Nodens::TimeStep ts) override;

private:
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