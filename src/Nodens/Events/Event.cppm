module;

export module Nodens.Events;

import std;

export namespace Nodens
{

/// @brief Enumeration of all supported event types.
enum class EventType
{
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
enum EventCategory
{
    None                     = 0,
    EventCategoryApplication = 1 << 0,
    EventCategoryInput       = 1 << 1,
    EventCategoryKeyboard    = 1 << 2,
    EventCategoryMouse       = 1 << 3,
    EventCategoryMouseButton = 1 << 4
};

/// @brief The abstract base class for all events.
class Event
{
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
    inline bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }

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
template <typename Derived, EventType Type> class EventImpl : public Event
{
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
class EventDispatcher
{
public:
    EventDispatcher(Event& event) : m_Event(event) {}

    /// @brief Dispatches the event to a handler function if the types match.
    /// @tparam T The concrete Event type to match against.
    /// @tparam F The function type.
    /// @param func A callback function taking T& and returning bool.
    /// @return True if the event types matched and the function was executed.
    template <IsEvent T, typename F> bool Dispatch(const F& func)
    {
        if (m_Event.GetEventType() == T::GetStaticType())
        {
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
inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
    return os << e.ToString();
}

// -------------------------------------------------------------------------
// KEY EVENTS
// -------------------------------------------------------------------------

/// @brief Abstract base class for all keyboard events.
/// @details Stores the common KeyCode used by Pressed, Released, and Typed events.
class KeyEvent : public Event
{
public:
    /// @brief Gets the standard key code associated with this event.
    inline int GetKeyCode() const { return m_KeyCode; }

    /// @brief Common category flags for all key events (Keyboard + Input).
    virtual int GetCategoryFlags() const override { return EventCategoryKeyboard | EventCategoryInput; }

protected:
    /// @brief Protected constructor to force usage of derived classes.
    KeyEvent(int keycode) : m_KeyCode(keycode) {}

    int m_KeyCode;
};

/// @brief Intermediate template to automate virtual method implementation for Key events.
/// @tparam Derived The concrete class type.
/// @tparam Type The EventType enum value.
template <typename Derived, EventType Type> class KeyEventImpl : public KeyEvent
{
public:
    /// @brief Static Type Accessor.
    static constexpr EventType GetStaticType() { return Type; }

    /// @brief Runtime Type Accessor.
    virtual EventType GetEventType() const override { return GetStaticType(); }

    /// @brief Name Accessor (retrieved from Derived class).
    virtual const char* GetName() const override { return Derived::Name; }

    // Pass keycode up to the abstract base
    KeyEventImpl(int keycode) : KeyEvent(keycode) {}
};

/// @brief Event triggered when a keyboard key is pressed down.
class KeyPressedEvent : public KeyEventImpl<KeyPressedEvent, EventType::KeyPressed>
{
public:
    static constexpr char Name[] = "KeyPressed";

    /// @brief Constructor.
    /// @param keycode The key being pressed.
    /// @param repeatCount 0 for initial press, >0 if the key is held down (OS auto-repeat).
    KeyPressedEvent(int keycode, int repeatCount) : KeyEventImpl(keycode), m_RepeatCount(repeatCount) {}

    /// @brief Gets the number of times this key event has repeated.
    inline int GetRepeatCount() const { return m_RepeatCount; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
        return ss.str();
    }

private:
    int m_RepeatCount;
};

/// @brief Event triggered when a keyboard key is lifted.
class KeyReleasedEvent : public KeyEventImpl<KeyReleasedEvent, EventType::KeyReleased>
{
public:
    static constexpr char Name[] = "KeyReleased";

    KeyReleasedEvent(int keycode) : KeyEventImpl(keycode) {}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyReleasedEvent: " << m_KeyCode;
        return ss.str();
    }
};

/// @brief Event triggered for text input.
/// @details Unlike KeyPressed, this is intended for character input (handling capitalization, etc.).
class KeyTypedEvent : public KeyEventImpl<KeyTypedEvent, EventType::KeyTyped>
{
public:
    static constexpr char Name[] = "KeyTyped";

    KeyTypedEvent(int keycode) : KeyEventImpl(keycode) {}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyTypedEvent: " << m_KeyCode;
        return ss.str();
    }
};

// -------------------------------------------------------------------------
// MOUSE EVENTS
// -------------------------------------------------------------------------

/// @brief Event triggered when the mouse cursor moves.
/// @details Contains the absolute X and Y coordinates relative to the window.
class MouseMovedEvent : public EventImpl<MouseMovedEvent, EventType::MouseMoved>
{
public:
    static constexpr char Name[]   = "MouseMoved";
    static constexpr int  Category = EventCategoryMouse | EventCategoryInput;

    MouseMovedEvent(float x, float y) : m_MouseX(x), m_MouseY(y) {}

    inline float GetX() const { return m_MouseX; }
    inline float GetY() const { return m_MouseY; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
        return ss.str();
    }

private:
    float m_MouseX, m_MouseY;
};

/// @brief Event triggered by the mouse wheel.
class MouseScrolledEvent : public EventImpl<MouseScrolledEvent, EventType::MouseScrolled>
{
public:
    static constexpr char Name[]   = "MouseScrolled";
    static constexpr int  Category = EventCategoryMouse | EventCategoryInput;

    /// @brief Constructor.
    /// @param xOffset The horizontal scroll amount (usually 0).
    /// @param yOffset The vertical scroll amount (wheel delta).
    MouseScrolledEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

    inline float GetXOffset() const { return m_XOffset; }
    inline float GetYOffset() const { return m_YOffset; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
        return ss.str();
    }

private:
    float m_XOffset, m_YOffset;
};

/// @brief Abstract base class for mouse button interactions.
class MouseButtonEvent : public Event
{
public:
    inline int GetMouseButton() const { return m_Button; }

    virtual int GetCategoryFlags() const override
    {
        return EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton;
    }

protected:
    MouseButtonEvent(int button) : m_Button(button) {}

    int m_Button;
};

/// @brief Intermediate template for Mouse Button events (Pressed/Released).
template <typename Derived, EventType Type> class MouseButtonEventImpl : public MouseButtonEvent
{
public:
    static constexpr EventType GetStaticType() { return Type; }
    virtual EventType          GetEventType() const override { return GetStaticType(); }
    virtual const char*        GetName() const override { return Derived::Name; }

    MouseButtonEventImpl(int button) : MouseButtonEvent(button) {}
};

/// @brief Event triggered when a mouse button is clicked down.
class MouseButtonPressedEvent : public MouseButtonEventImpl<MouseButtonPressedEvent, EventType::MouseButtonPressed>
{
public:
    static constexpr char Name[] = "MouseButtonPressed";

    MouseButtonPressedEvent(int button) : MouseButtonEventImpl(button) {}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseButtonPressedEvent: " << m_Button;
        return ss.str();
    }
};

/// @brief Event triggered when a mouse button is released.
class MouseButtonReleasedEvent : public MouseButtonEventImpl<MouseButtonReleasedEvent, EventType::MouseButtonReleased>
{
public:
    static constexpr char Name[] = "MouseButtonReleased";

    MouseButtonReleasedEvent(int button) : MouseButtonEventImpl(button) {}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseButtonReleasedEvent: " << m_Button;
        return ss.str();
    }
};

// -------------------------------------------------------------------------
// APPLICATION EVENTS
// -------------------------------------------------------------------------

/// @brief Event triggered when the native window is resized.
/// @details Contains the new width and height of the window.
class WindowResizeEvent : public EventImpl<WindowResizeEvent, EventType::WindowResize>
{
public:
    /// @brief The debug name of this event.
    static constexpr char Name[] = "WindowResize";

    /// @brief The categories this event belongs to.
    static constexpr int Category = EventCategoryApplication;

    /// @brief Constructs a resize event.
    /// @param width The new width of the window.
    /// @param height The new height of the window.
    WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}

    /// @brief Gets the new window width.
    inline unsigned int GetWidth() const { return m_Width; }

    /// @brief Gets the new window height.
    inline unsigned int GetHeight() const { return m_Height; }

    /// @brief Debug string representation.
    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
        return ss.str();
    }

private:
    unsigned int m_Width, m_Height;
};

/// @brief Event triggered when the user attempts to close the window.
/// @details This is usually dispatched before the application actually shuts down,
/// allowing systems to save state or cancel the close.
class WindowCloseEvent : public EventImpl<WindowCloseEvent, EventType::WindowClose>
{
public:
    static constexpr char Name[]   = "WindowClose";
    static constexpr int  Category = EventCategoryApplication;

    WindowCloseEvent() = default;
};

/// @brief Event triggered every fixed simulation step.
/// @details Useful for physics or fixed-timestep logic logic.
class AppTickEvent : public EventImpl<AppTickEvent, EventType::AppTick>
{
public:
    static constexpr char Name[]   = "AppTick";
    static constexpr int  Category = EventCategoryApplication;

    AppTickEvent() = default;
};

/// @brief Event triggered once per frame update.
/// @details Use this for variable time-step logic (Input polling, Camera movement).
class AppUpdateEvent : public EventImpl<AppUpdateEvent, EventType::AppUpdate>
{
public:
    static constexpr char Name[]   = "AppUpdate";
    static constexpr int  Category = EventCategoryApplication;

    AppUpdateEvent() = default;
};

/// @brief Event triggered when the application is ready to render.
/// @details This is often used to synchronize ImGui rendering or custom draw passes.
class AppRenderEvent : public EventImpl<AppRenderEvent, EventType::AppRender>
{
public:
    static constexpr char Name[]   = "AppRender";
    static constexpr int  Category = EventCategoryApplication;

    AppRenderEvent() = default;
};

} // namespace Nodens