#pragma once

#include <concepts>  // C++20: Required for 'concept' keyword
#include <functional>
#include <sstream>
#include <string>

#include "Nodens/Core.h"
#include "ndpch.h"

namespace Nodens {

/// @brief Enumeration of all supported event types.
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
  KeyTyped,
  MouseButtonPressed,
  MouseButtonReleased,
  MouseMoved,
  MouseScrolled
};

/// @brief Bitfield flags for categorizing events.
/// @details Allows filtering events broadly (e.g., "log all Input events").
enum EventCategory {
  None = 0,
  EventCategoryApplication = Bit(0),
  EventCategoryInput = Bit(1),
  EventCategoryKeyboard = Bit(2),
  EventCategoryMouse = Bit(3),
  EventCategoryMouseButton = Bit(4)
};

/// @brief The abstract base class for all events.
class Event {
  friend class EventDispatcher;

 public:
  virtual ~Event() = default;

  /// @brief Gets the specific runtime type of the event.
  virtual EventType GetEventType() const = 0;

  /// @brief Gets the debug name of the event.
  virtual const char* GetName() const = 0;

  /// @brief Gets the category flags associated with this event.
  virtual int GetCategoryFlags() const = 0;

  /// @brief Returns a string representation of the event for debugging.
  virtual std::string ToString() const { return GetName(); }

  /// @brief Checks if this event belongs to a specific category.
  inline bool IsInCategory(EventCategory category) {
    return GetCategoryFlags() & category;
  }

 public:
  /// @brief Flag indicating if the event has been handled.
  /// @details If true, subsequent layers usually ignore this event.
  bool Handled = false;
};

// -------------------------------------------------------------------------
// C++20 CRTP BASE CLASS
// Replaces the old EVENT_CLASS_TYPE and EVENT_CLASS_CATEGORY macros
// -------------------------------------------------------------------------

/// @brief A Helper class to automate the implementation of virtual functions.
/// @tparam Derived The concrete event class (e.g., WindowResizeEvent).
/// @tparam Type The EventType enum value for this class.
template <typename Derived, EventType Type>
class EventImpl : public Event {
 public:
  /// @brief Static access to the event type, required by EventDispatcher.
  static constexpr EventType GetStaticType() { return Type; }

  /// @brief Implements pure virtual GetEventType() from base Event.
  virtual EventType GetEventType() const override { return GetStaticType(); }

  /// @brief Implements pure virtual GetName() using static data in Derived.
  virtual const char* GetName() const override { return Derived::Name; }

  /// @brief Implements pure virtual GetCategoryFlags() using static data in
  /// Derived.
  virtual int GetCategoryFlags() const override { return Derived::Category; }
};

// -------------------------------------------------------------------------
// C++20 DISPATCHER
// -------------------------------------------------------------------------

/// @brief A Concept ensuring type T inherits from the base Event class.
template <typename T>
concept IsEvent = std::derived_from<T, Event>;

/// @brief Utility class for dispatching events based on their runtime type.
class EventDispatcher {
 public:
  EventDispatcher(Event& event) : m_Event(event) {}

  /// @brief Dispatches the event to a handler function if the types match.
  /// @tparam T The concrete Event type to match against.
  /// @tparam F The function type.
  /// @param func A callback function taking T& and returning bool.
  /// @return True if the event types matched and the function was executed.
  template <IsEvent T, typename F>
  bool Dispatch(const F& func) {
    if (m_Event.GetEventType() == T::GetStaticType()) {
      // Safe cast because we verified the type enum above.
      m_Event.Handled |= func(static_cast<T&>(m_Event));
      return true;
    }
    return false;
  }

 private:
  Event& m_Event;
};

/// @brief Operator overload for easy logging of events.
inline std::ostream& operator<<(std::ostream& os, const Event& e) {
  return os << e.ToString();
}

}  // namespace Nodens