#pragma once

#include <Nodens.h>

#include <future>

class JobSystemLayer : public Nodens::Layer {
 public:
  JobSystemLayer();
  virtual ~JobSystemLayer() = default;

  virtual void OnAttach() override;
  virtual void OnDetach() override;

  // We check for job completion here
  virtual void OnUpdate(Nodens::TimeStep ts) override;

  // We draw the UI and "Start" button here
  virtual void OnImGuiRender(Nodens::TimeStep ts) override;

  virtual void OnEvent(Nodens::Event& e) override;

 private:
  // The future holds the result of our background task
  std::future<int> m_JobFuture;

  bool m_IsJobRunning = false;
  int m_JobResult = 0;
  float m_TimePassed = 0.0f;
};