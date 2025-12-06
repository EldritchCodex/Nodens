#include "AsyncEventBus.h"

#include "Nodens/Application.h"
#include "ndpch.h"
#include <tracy/Tracy.hpp>

namespace Nodens
{

AsyncEventBus& AsyncEventBus::Get()
{
    static AsyncEventBus instance;
    return instance;
}

void AsyncEventBus::SubscribeInternal(std::type_index type, EventHandler handler)
{
    // Lock just long enough to add the handler to the list
    std::scoped_lock lock(m_Mutex);
    m_Subscribers[type].push_back(handler);
}

void AsyncEventBus::PublishInternal(std::shared_ptr<Event> event)
{
    // Profile the act of submitting (usually fast)
    ZoneScoped;

    Application::Get().GetJobSystem().Submit(
        [this, event]()
        {
            // Profile the asynchronous execution (the actual work)
            ZoneScoped;
            const char* name = event->GetName();
            ZoneName(name, strlen(name));

            std::vector<EventHandler> handlers;
            {
                std::scoped_lock lock(m_Mutex);
                auto             it = m_Subscribers.find(typeid(*event));
                if (it != m_Subscribers.end())
                {
                    handlers = it->second;
                }
            }

            for (auto& handler : handlers)
            {
                handler(*event);
            }
        });
}

} // namespace Nodens