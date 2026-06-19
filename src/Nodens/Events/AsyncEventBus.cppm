/// @file AsyncEventBus.cppm
/// @brief Thread-safe, asynchronous publish/subscribe event bus.
/// @details Events published through this bus are dispatched asynchronously on the
///          application's JobSystem worker threads, decoupling the publisher from
///          subscriber execution.
/// @ingroup Events

export module Nodens.AsyncEventBus;

import Nodens.Events;
import std;

export namespace Nodens
{

/// @brief A singleton publish/subscribe event bus that dispatches events asynchronously.
/// @details Subscribers register type-erased handlers via Subscribe<T>(). When Publish<T>()
///          is called, the event is copied into a shared_ptr and submitted to the JobSystem,
///          where matching handlers are invoked on a worker thread.
///
///          This is useful for fire-and-forget notifications where the publisher does not
///          need to wait for handlers to complete.
///
/// @note All handler invocations are thread-safe -- the subscriber list is protected by a mutex.
/// @warning Handlers execute on worker threads; they must not access non-thread-safe resources
///          without their own synchronization.
/// @see JobSystem, Event
/// @ingroup Events
class AsyncEventBus
{
public:
    /// @brief Type alias for a type-erased event handler function.
    using EventHandler = std::function<void(Event&)>;

    /// @brief Returns the singleton AsyncEventBus instance.
    /// @return Reference to the global AsyncEventBus.
    static AsyncEventBus& Get();

    /// @brief Subscribes a strongly-typed handler for events of type T.
    /// @tparam T The concrete Event type to listen for.
    /// @tparam F The callable type of the handler.
    /// @param handler A callback (invocable with T&) that receives a reference to the event.
    /// @details The handler is wrapped in a type-erased EventHandler and stored
    ///          internally, keyed by `typeid(T)`.
    template <IsEvent T, std::invocable<T&> F>
    void Subscribe(F&& handler)
    {
        auto wrapper = [handler = std::forward<F>(handler)](Event& e) { handler(static_cast<T&>(e)); };

        SubscribeInternal(typeid(T), wrapper);
    }

    /// @brief Publishes an event asynchronously to all registered subscribers.
    /// @tparam T The concrete Event type to publish.
    /// @param event The event value to broadcast. It is copied into a shared_ptr.
    /// @details The event is submitted to the JobSystem and handlers are invoked
    ///          on a worker thread. This call returns immediately.
    template <IsEvent T>
    void Publish(T event)
    {
        auto eventPtr = std::make_shared<T>(event);
        PublishInternal(eventPtr);
    }

private:
    /// @brief Private constructor — use Get() to access the singleton.
    AsyncEventBus() = default;

    /// @brief Registers a type-erased handler for a given type index.
    /// @param type The type_index key for the event type.
    /// @param handler The type-erased handler to store.
    void SubscribeInternal(std::type_index type, EventHandler handler);

    /// @brief Submits the event to the JobSystem for asynchronous handler invocation.
    /// @param event Shared pointer to the event to dispatch.
    void PublishInternal(std::shared_ptr<Event> event);

private:
    /// @brief Map from event type to list of registered handlers.
    std::unordered_map<std::type_index, std::vector<EventHandler>> m_Subscribers;

    /// @brief Mutex protecting concurrent access to m_Subscribers.
    std::mutex m_Mutex;
};

} // namespace Nodens
