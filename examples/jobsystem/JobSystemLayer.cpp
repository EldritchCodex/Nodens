#include "JobSystemLayer.h"

#include <imgui.h>

#include <chrono>
#include <thread>

JobSystemLayer::JobSystemLayer() : Layer("JobSystemLayer") {}

void JobSystemLayer::OnUpdate(Nodens::TimeStep ts) {
  m_TimePassed += ts;

  // ==============================================================
  // POLL: Check if the background job is finished
  // ==============================================================
  // We use wait_for with 0 seconds to check status without blocking
  // the rendering thread.
  if (m_IsJobRunning && m_JobFuture.valid()) {
    if (m_JobFuture.wait_for(std::chrono::seconds(0)) ==
        std::future_status::ready) {
      // 1. Retrieve the result (this moves the data to main thread)
      m_JobResult = m_JobFuture.get();

      // 2. Mark as done
      m_IsJobRunning = false;
      ND_INFO("Background job finished! Result: {0}", m_JobResult);
    }
  }
}
void JobSystemLayer::OnImGuiRender(Nodens::TimeStep ts) {
  ND_PROFILE_ZONE_SCOPED;

  ImGui::Begin("Job System Control Panel");
  ImGui::Text("Application Average: %.3f ms/frame (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  ImGui::Text("Active Time: %.2f s", m_TimePassed);
  ImGui::Separator();

  if (m_IsJobRunning) {
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),
                       "STATUS: Processing Heavy Job...");
  } else {
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "STATUS: Idle");

    if (ImGui::Button("Start Heavy Calculation (2s sleep)")) {
      m_IsJobRunning = true;
      m_JobResult = 0;

      auto& jobSystem = Nodens::Application::Get().GetJobSystem();

      m_JobFuture = jobSystem.Submit([]() {
        ND_PROFILE_ZONE_NAMED("Heavy Calculation");

        ND_INFO("Thread: Job started...");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        ND_INFO("Thread: Job finished!");

        return 42;
      });
    }
  }

  ImGui::Separator();
  ImGui::Text("Result from Thread: %d", m_JobResult);
  ImGui::End();
}

void JobSystemLayer::OnEvent(Nodens::Event& e) {}