export module Nodens.AsyncEventBus;

import Nodens.Events;
import std;

export namespace Nodens
{

class AsyncEventBus
{
public:
    using EventHandler = std::function<void(Event&)>;

    static AsyncEventBus& Get();

    template <typename T> void Subscribe(const std::function<void(T&)>& handler)
    {
        auto wrapper = [handler](Event& e) { handler(static_cast<T&>(e)); };

        SubscribeInternal(typeid(T), wrapper);
    }

    template <typename T> void Publish(T event)
    {
        auto eventPtr = std::make_shared<T>(event);
        PublishInternal(eventPtr);
    }

private:
    AsyncEventBus() = default;
    void SubscribeInternal(std::type_index type, EventHandler handler);
    void PublishInternal(std::shared_ptr<Event> event);

private:
    std::unordered_map<std::type_index, std::vector<EventHandler>> m_Subscribers;
    std::mutex                                                     m_Mutex;
};

} // namespace Nodens
