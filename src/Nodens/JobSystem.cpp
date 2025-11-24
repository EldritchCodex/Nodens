#include "JobSystem.h"

#include "Nodens/Profiling.h"
#include "ndpch.h"

namespace Nodens
{

JobSystem::JobSystem()
{
    // Calculate how many threads to spawn. We generally want to match the CPU
    // cores but leave one core free for the main thread to prevent hitching.
    unsigned int threadCount = std::thread::hardware_concurrency();
    if (threadCount > 1)
        threadCount--;

    m_Threads.reserve(threadCount);

    // Launch the workers.
    for (unsigned int i = 0; i < threadCount; ++i)
    {
        // std::jthread automatically passes a stop_token as the first argument
        // to the thread function if the signature accepts it.
        m_Threads.emplace_back(
            [this, i](std::stop_token stoken)
            {
                // Profiler hook: Give the thread a name so we can see it when using
                // Tracy.
                std::string name = "Worker " + std::to_string(i);
                ND_PROFILE_SET_THREAD_NAME(name.c_str());

                this->WorkerLoop(stoken);
            });
    }

    ND_CORE_INFO("JobSystem initialized with {0} worker threads.", threadCount);
}

JobSystem::~JobSystem()
{
    // SHUTDOWN SEQUENCE
    // Ask all threads to stop. Ideally, they finish their current task and then
    // see this.
    for (auto& thread : m_Threads)
    {
        thread.request_stop();
    }

    // Wake up everyone, because if a thread is sleeping in 'm_Condition.wait', it
    // won't see the stop request until it wakes up. notify_all() forces them to
    // wake, check the token, and exit.
    m_Condition.notify_all();

    // Because they are std::jthread, they will automatically perform a .join()
    // waiting for the threads to actually finish execution before this object
    // dies.
}

void JobSystem::WorkerLoop(std::stop_token stoken)
{
    while (!stoken.stop_requested())
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);

            // Wait until there is a task or we are stopped.
            // condition_variable_any::wait accepts a stop_token.
            // It handles the lock/unlock/relock dance automatically.
            bool tasksAvailable = m_Condition.wait(lock, stoken, [this] { return !m_Tasks.empty(); });

            // If we woke up but there are no tasks (meaning stop was requested
            // and the predicate returned false), we should exit.
            // Note: 'tasksAvailable' is the result of the predicate.
            if (!tasksAvailable && stoken.stop_requested())
            {
                return;
            }

            // Double check queue size to be safe
            if (m_Tasks.empty())
            {
                continue;
            }

            task = std::move(m_Tasks.front());
            m_Tasks.pop();
        }

        // Execute outside the lock
        {
            ND_PROFILE_ZONE_SCOPED;
            if (task)
                task();
        }
    }
}

} // namespace Nodens