#include "JobSystem.h"

#include "Nodens/Profiling.h"
#include "ndpch.h"

namespace Nodens {

JobSystem::JobSystem() : m_Stop(false) {
  unsigned int threadCount = std::thread::hardware_concurrency();
  if (threadCount > 1) threadCount--;

  for (unsigned int i = 0; i < threadCount; ++i) {
    m_Threads.emplace_back([this, i] {
      // Naming the thread makes it show up as "Worker {i}" when profiling
      std::string name = "Worker " + std::to_string(i);
      ND_PROFILE_SET_THREAD_NAME(name.c_str());

      this->WorkerLoop();
    });
  }
}

JobSystem::~JobSystem() {
  // 1. Signal worker threads to stop
  {
    std::unique_lock<std::mutex> lock(m_QueueMutex);
    m_Stop = true;
  }

  // 2. Wake up all waiting threads
  m_Condition.notify_all();

  // 3. Wait for all threads to finish
  for (std::thread& worker : m_Threads) {
    if (worker.joinable()) worker.join();
  }
}

void JobSystem::WorkerLoop() {
  while (true) {
    std::function<void()> task;
    {
      std::unique_lock<std::mutex> lock(m_QueueMutex);
      m_Condition.wait(lock, [this] { return m_Stop || !m_Tasks.empty(); });

      if (m_Stop && m_Tasks.empty()) return;

      task = std::move(m_Tasks.front());
      m_Tasks.pop();
    }
    {
      ND_PROFILE_ZONE_SCOPED;
      task();
    }
  }
}

}  // namespace Nodens