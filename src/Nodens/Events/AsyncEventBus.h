#pragma once

#include "Nodens/Events/Event.h"
#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <mutex>
#include <memory>

namespace Nodens {

class AsyncEventBus {
public:
    // Define a generic handler type
    using EventHandler = std::function<void(Event&)>;

    // Singleton Access
    static AsyncEventBus& Get();

    // ==================================================================
    // 1. SUBSCRIBE
    // Systems call this to listen for specific events (e.g., "CollisionEvent")
    // ==================================================================
    template<typename T>
    void Subscribe(const std::function<void(T&)>& handler) {
        // We wrap the specific handler (T&) into a generic one (Event&)
        // This allows us to store all handlers in a single generic list.
        auto wrapper = [handler](Event& e) {
            // Safe cast because we map this wrapper to typeid(T) below
            handler(static_cast<T&>(e));
        };

        SubscribeInternal(typeid(T), wrapper);
    }

    // ==================================================================
    // 2. PUBLISH (Fire and Forget)
    // Systems call this to broadcast an event. It returns IMMEDIATELY.
    // The actual processing happens later on a background thread.
    // ==================================================================
    template<typename T>
    void Publish(T event) {
        // CRITICAL: We must COPY the event into a smart pointer.
        // The original 'event' variable on the stack will be destroyed
        // immediately after this function returns, but the thread needs
        // to access it milliseconds later.
        auto eventPtr = std::make_shared<T>(event);

        PublishInternal(eventPtr);
    }

private:
    // Hidden implementation details to keep header clean
    AsyncEventBus() = default;
    void SubscribeInternal(std::type_index type, EventHandler handler);
    void PublishInternal(std::shared_ptr<Event> event);

private:
    // Map Key: The type of event (e.g., Type of 'PlayerJumpEvent')
    // Map Value: A list of functions to call
    std::unordered_map<std::type_index, std::vector<EventHandler>> m_Subscribers;

    std::mutex m_Mutex;
};

}