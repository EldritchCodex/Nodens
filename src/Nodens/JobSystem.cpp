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
    while (true)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);

            // C++20 WAIT LOGIC:
            // The wait() function here does three things atomically:
            // 1. Unlocks the mutex.
            // 2. Checks the predicate: "Are there tasks?" OR "Is stop requested?"
            // 3. If neither, it sleeps.
            //
            // If stop is requested, 'wait' returns false immediately.
            bool active = m_Condition.wait(lock, stoken, [this] { return !m_Tasks.empty(); });

            // If wait returned false, it means a stop was requested and the predicate
            // failed. However, we must be careful: we might have been stopped BUT
            // there are still tasks left. Ideally, we might want to finish the queue.
            // For now, we exit immediately if stopped and empty.
            if (!active && m_Tasks.empty())
            {
                return;
            }

            // If we are here, we have the lock and there is a task (or we are
            // stopping but clearing the queue). Double check queue isn't empty to be
            // safe.
            if (m_Tasks.empty())
            {
                if (stoken.stop_requested())
                    return;
                continue; // Spurious wake-up protection
            }

            task = std::move(m_Tasks.front());
            m_Tasks.pop();
        } // Lock releases here

        // Execute the task outside the lock so other threads aren't blocked.
        {
            ND_PROFILE_ZONE_SCOPED;
            task();
        }
    }
}

} // namespace Nodens