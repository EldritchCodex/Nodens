#pragma once

#include <functional>
#include <ostream>
#include <string>

#include "Nodens/Core.h"

namespace Nodens {
// Events are currently blocking, meaning when an event occurs it
// immediately gets dispatched and must be dealt with right then and there.
// For the future, a better strategy might be to buffer events in an event
// bus and process them during the "event" part of the update stage.

enum class EventType {
  None = 0,
  WindowClose,
  WindowResize,
  WindowFocus,
  WindowLostFocus,
  WindowMoved,
  AppTick,
  AppUpdate,
  AppRender,
  KeyPressed,
  KeyReleased,
  MouseButtonPressed,
  MouseButtonReleased,
  MouseMoved,
  MouseScrolled
};

enum EventCategory {
  None = 0,
  EventCategoryApplication = BIT(0),
  EventCategoryInput = BIT(1),
  EventCategoryKeyboard = BIT(2),
  EventCategoryMouse = BIT(3),
  EventCategoryMouseButton = BIT(4)
};

#define EVENT_CLASS_TYPE(type)                                                \
  static EventType GetStaticType() { return EventType::##type; }              \
  virtual EventType GetEventType() const override { return GetStaticType(); } \
  virtual const char* GetName() const override { return #type; }
// #  -> Stringizing operator: Causes the corresponding actual argument to be
// enclosed in double quotation marks
// ## -> Token-pasting operator: Allows tokens used as actual arguments to be
// concatenated to form other tokens

#define EVENT_CLASS_CATEGORY(category) \
  virtual int GetCategoryFlags() const override { return category; }

class Event {
 public:
  virtual ~Event() = default;

  bool Handled = false;

  virtual EventType GetEventType() const = 0;  // Pure virtual
  virtual const char* GetName() const = 0;     // Pure virtual
  virtual int GetCategoryFlags() const = 0;    // Pure virtual
  virtual std::string ToString() const { return GetName(); }

  inline bool IsInCategory(EventCategory category) {
    return GetCategoryFlags() & category;
  }
};

class EventDispatcher {
 public:
  EventDispatcher(Event& event) : m_Event(event) {}

  // F will be resolved at compile time
  template <typename T, typename F>
  bool Dispatch(const F& func) {
    if (m_Event.GetEventType() == T::GetStaticType()) {
      m_Event.Handled |= func(static_cast<T&>(m_Event));
      return true;
    }
    return false;
  }

 private:
  Event& m_Event;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e) {
  return os << e.ToString();
}

}  // namespace Nodens