export module Example.JobSystemLayer;

import Nodens;
import std;

// -----------------------------------------------------------------------------
// Step 1: Define the Layer class. This layer will be responsible for
// managing the state of the job system and rendering it using ImPlot
// and the Nodens GUI infrastructure.
// -----------------------------------------------------------------------------

export class JobSystemLayer : public Nodens::Layer
{
public:
    JobSystemLayer();
    ~JobSystemLayer() override = default;

    void OnUpdate(Nodens::TimeStep ts) override;
    void OnImGuiRender(Nodens::TimeStep ts) override;
    void OnEvent(Nodens::Event& e) override;

private:
    std::future<int> m_JobFuture;

    bool  m_IsJobRunning{};
    int   m_JobResult{};
    float m_TimePassed{};
};
