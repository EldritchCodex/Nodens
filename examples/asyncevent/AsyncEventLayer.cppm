export module Example.AsyncEventLayer;

import Example.AsyncEvent;
import Nodens.AsyncEventBus;
import Nodens.Layer;
import Nodens.TimeStep;
import std;

export class AsyncEventLayer : public Nodens::Layer
{
public:
    AsyncEventLayer();
    ~AsyncEventLayer() override = default;

    void OnAttach() override;
    void OnUpdate(Nodens::TimeStep ts) override;
    void OnImGuiRender(Nodens::TimeStep ts) override;

private:
    void AddResult(const PlanetaryScanEvent& e);

private:
    float m_TimePassed  = 0.0f;
    int   m_ScanCounter = 0;

    std::mutex m_DataMutex;

    std::vector<float> m_GalaxyDistances;
    std::vector<float> m_GalaxyDensities;

    std::vector<float> m_LatencyHistory;
    float              m_MaxLatency = 0.0f;
};
