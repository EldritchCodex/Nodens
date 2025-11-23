#include "AsyncEventBus.h"

#include "Nodens/Application.h"
#include "Nodens/Profiling.h"
#include "ndpch.h"

namespace Nodens {

AsyncEventBus& AsyncEventBus::Get() {
  static AsyncEventBus instance;
  return instance;
}

void AsyncEventBus::SubscribeInternal(std::type_index type,
                                      EventHandler handler) {
  // Lock just long enough to add the handler to the list
  std::scoped_lock lock(m_Mutex);
  m_Subscribers[type].push_back(handler);
}

void AsyncEventBus::PublishInternal(std::shared_ptr<Event> event) {
  // Profile the act of submitting (usually fast)
  ND_PROFILE_ZONE_SCOPED;

  Application::Get().GetJobSystem().Submit([this, event]() {
    // Profile the asynchronous execution (the actual work)
    // We name the zone dynamically based on the event type
    ND_PROFILE_ZONE_NAMED_DYNAMIC(event->GetName());

    std::vector<EventHandler> handlers;
    {
      std::scoped_lock lock(m_Mutex);
      auto it = m_Subscribers.find(typeid(*event));
      if (it != m_Subscribers.end()) {
        handlers = it->second;
      }
    }

    for (auto& handler : handlers) {
      handler(*event);
    }
  });
}

}  // namespace Nodens