#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace Nodens {

class JobSystem {
 public:
  JobSystem();
  ~JobSystem();

  template <class F, class... Args>
  auto Submit(F&& f, Args&&... args)
      -> std::future<typename std::invoke_result<F, Args...>::type> {
    using return_type = typename std::invoke_result<F, Args...>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
      std::unique_lock<std::mutex> lock(m_QueueMutex);
      if (m_Stop) throw std::runtime_error("Enqueue on stopped JobSystem");

      m_Tasks.emplace([task]() { (*task)(); });
    }
    m_Condition.notify_one();
    return res;
  }

 private:
  // We moved the loop logic to a private function
  void WorkerLoop();

 private:
  std::vector<std::thread> m_Threads;
  std::queue<std::function<void()>> m_Tasks;

  std::mutex m_QueueMutex;
  std::condition_variable m_Condition;
  bool m_Stop;
};

}  // namespace Nodens