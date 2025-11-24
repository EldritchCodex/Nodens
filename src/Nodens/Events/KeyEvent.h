#pragma once

#include "Nodens/Events/Event.h"
#include "Nodens/KeyCodes.h" // Assuming you have your key codes defined here

namespace Nodens {

    // -------------------------------------------------------------------------
    // KEY EVENT BASE (Abstract)
    // -------------------------------------------------------------------------

    /// @brief Abstract base class for all keyboard events.
    /// @details Stores the common KeyCode used by Pressed, Released, and Typed events.
    class KeyEvent : public Event
    {
    public:
        /// @brief Gets the standard key code associated with this event.
        inline int GetKeyCode() const { return m_KeyCode; }

        /// @brief Common category flags for all key events (Keyboard + Input).
        virtual int GetCategoryFlags() const override
        {
            return EventCategoryKeyboard | EventCategoryInput;
        }

    protected:
        /// @brief Protected constructor to force usage of derived classes.
        KeyEvent(int keycode)
            : m_KeyCode(keycode) {}

        int m_KeyCode;
    };

    // -------------------------------------------------------------------------
    // KEY EVENT IMPLEMENTATION HELPER
    // -------------------------------------------------------------------------

    /// @brief Intermediate template to automate virtual method implementation for Key events.
    /// @tparam Derived The concrete class type.
    /// @tparam Type The EventType enum value.
    template<typename Derived, EventType Type>
    class KeyEventImpl : public KeyEvent
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

    // -------------------------------------------------------------------------
    // KEY PRESSED
    // -------------------------------------------------------------------------

    /// @brief Event triggered when a keyboard key is pressed down.
    class KeyPressedEvent : public KeyEventImpl<KeyPressedEvent, EventType::KeyPressed>
    {
    public:
        static constexpr char Name[] = "KeyPressed";

        /// @brief Constructor.
        /// @param keycode The key being pressed.
        /// @param repeatCount 0 for initial press, >0 if the key is held down (OS auto-repeat).
        KeyPressedEvent(int keycode, int repeatCount)
            : KeyEventImpl(keycode), m_RepeatCount(repeatCount) {}

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

    // -------------------------------------------------------------------------
    // KEY RELEASED
    // -------------------------------------------------------------------------

    /// @brief Event triggered when a keyboard key is lifted.
    class KeyReleasedEvent : public KeyEventImpl<KeyReleasedEvent, EventType::KeyReleased>
    {
    public:
        static constexpr char Name[] = "KeyReleased";

        KeyReleasedEvent(int keycode)
            : KeyEventImpl(keycode) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_KeyCode;
            return ss.str();
        }
    };

    // -------------------------------------------------------------------------
    // KEY TYPED
    // -------------------------------------------------------------------------

    /// @brief Event triggered for text input.
    /// @details Unlike KeyPressed, this is intended for character input (handling capitalization, etc.).
    class KeyTypedEvent : public KeyEventImpl<KeyTypedEvent, EventType::KeyTyped>
    {
    public:
        static constexpr char Name[] = "KeyTyped";

        KeyTypedEvent(int keycode)
            : KeyEventImpl(keycode) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_KeyCode;
            return ss.str();
        }
    };
}