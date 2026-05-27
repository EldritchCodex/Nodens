#pragma once

#include "nodens.h"

#include <future>

/// @brief Layer for the JobSystem example.
/// This layer demonstrates how to use the Nodens::JobSystem to run a background task.
class JobSystemLayer : public Nodens::Layer
{
public:
    /// @brief Constructs the JobSystemLayer.
    JobSystemLayer();

    /// @brief Destroys the JobSystemLayer.
    virtual ~JobSystemLayer() = default;

    /// @brief Called every frame to update the layer.
    /// We check for job completion here.
    /// @param ts The time since the last frame.
    virtual void OnUpdate(Nodens::TimeStep ts) override;

    /// @brief Called every frame to render the ImGui UI.
    /// We draw the UI and "Start" button here.
    /// @param ts The time since the last frame.
    virtual void OnImGuiRender(Nodens::TimeStep ts) override;

    /// @brief Called when an event is dispatched to the layer.
    /// @param e The event to be handled.
    virtual void OnEvent(Nodens::Event& e) override;

private:
    // The future holds the result of our background task
    std::future<int> m_JobFuture;

    bool  m_IsJobRunning = false;
    int   m_JobResult    = 0;
    float m_TimePassed   = 0.0f;
};