module;

#include <tracy/Tracy.hpp>

module Nodens.AsyncEventBus;

import Nodens;
import std;

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
    std::lock_guard lock(m_Mutex);
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
            ZoneName(name, std::strlen(name));

            std::vector<EventHandler> handlers;
            {
                std::lock_guard lock(m_Mutex);
                Event*          rawEvent = event.get();
                auto            it       = m_Subscribers.find(typeid(*rawEvent));
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