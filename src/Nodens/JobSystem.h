#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stop_token>
#include <thread>
#include <vector>

namespace Nodens {

/// @brief A multithreaded job system utilizing C++20 jthreads for automatic joining and cancellation.
/// @details This system manages a pool of worker threads that consume tasks from a shared queue.
/// It uses a 'future-based' approach for task submission.
class JobSystem {
 public:
  /// @brief Initializes the JobSystem and launches worker threads.
  /// @details The number of threads spawned is equal to hardware_concurrency - 1
  /// to leave the main thread free for the OS/Application loop.
  JobSystem();

  /// @brief Destructor.
  /// @details Signals all threads to stop, wakes them up, and waits for them to join.
  /// Due to std::jthread, the joining happens automatically, but we explicitly signal stop first.
  ~JobSystem();

  /// @brief Submits a function (job) to the execution queue.
  /// @tparam F The type of the function object.
  /// @tparam Args The types of the arguments to pass to the function.
  /// @param f The function to execute.
  /// @param args The arguments to forward to the function.
  /// @return A std::future containing the result of the function execution.
  template <class F, class... Args>
  auto Submit(F&& f, Args&&... args)
      -> std::future<typename std::invoke_result<F, Args...>::type> {

    // Determine the return type of the submitted function F
    using return_type = typename std::invoke_result<F, Args...>::type;

    // Wrap the task in a packaged_task so we can get a future back.
    // 'std::bind' creates a callable object that binds the arguments to the function.
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
      // Lock the queue to safely add the new task
      std::unique_lock<std::mutex> lock(m_QueueMutex);

      // We wrap the task in a generic void lambda because the queue
      // only holds std::function<void()>.
      m_Tasks.emplace([task]() { (*task)(); });
    }

    // Wake up exactly one worker thread to handle this new task.
    m_Condition.notify_one();
    return res;
  }

 private:
  /// @brief The main loop executed by every worker thread.
  /// @param stoken The C++20 stop_token used to check if a stop has been requested.
  void WorkerLoop(std::stop_token stoken);

 private:
  /// @brief Pool of worker threads.
  /// @note std::jthread (C++20) automatically joins on destruction.
  std::vector<std::jthread> m_Threads;

  /// @brief The thread-safe queue of tasks pending execution.
  std::queue<std::function<void()>> m_Tasks;

  /// @brief Mutex to protect access to m_Tasks.
  std::mutex m_QueueMutex;

  /// @brief Condition variable to put threads to sleep when there is no work.
  /// @note std::condition_variable_any is required to work with std::stop_token.
  std::condition_variable_any m_Condition;
};

}  // namespace Nodens