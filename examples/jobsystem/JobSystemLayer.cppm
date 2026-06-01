export module Example.JobSystemLayer;

import Nodens.Application;
import Nodens.Events;
import Nodens.Layer;
import Nodens.TimeStep;
import std;

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

    bool  m_IsJobRunning = false;
    int   m_JobResult    = 0;
    float m_TimePassed   = 0.0f;
};
