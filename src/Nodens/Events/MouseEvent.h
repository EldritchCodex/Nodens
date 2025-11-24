#pragma once

#include "Nodens/Events/Event.h"
#include "Nodens/MouseButtonCodes.h" // Assuming you have mouse codes defined here
#include <sstream>

namespace Nodens {

    // -------------------------------------------------------------------------
    // MOUSE MOVED
    // -------------------------------------------------------------------------

    /// @brief Event triggered when the mouse cursor moves.
    /// @details Contains the absolute X and Y coordinates relative to the window.
    class MouseMovedEvent : public EventImpl<MouseMovedEvent, EventType::MouseMoved>
    {
    public:
        static constexpr char Name[] = "MouseMoved";
        static constexpr int Category = EventCategoryMouse | EventCategoryInput;

        MouseMovedEvent(float x, float y)
            : m_MouseX(x), m_MouseY(y) {}

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

    // -------------------------------------------------------------------------
    // MOUSE SCROLLED
    // -------------------------------------------------------------------------

    /// @brief Event triggered by the mouse wheel.
    class MouseScrolledEvent : public EventImpl<MouseScrolledEvent, EventType::MouseScrolled>
    {
    public:
        static constexpr char Name[] = "MouseScrolled";
        static constexpr int Category = EventCategoryMouse | EventCategoryInput;

        /// @brief Constructor.
        /// @param xOffset The horizontal scroll amount (usually 0).
        /// @param yOffset The vertical scroll amount (wheel delta).
        MouseScrolledEvent(float xOffset, float yOffset)
            : m_XOffset(xOffset), m_YOffset(yOffset) {}

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

    // -------------------------------------------------------------------------
    // MOUSE BUTTON BASE (Abstract)
    // -------------------------------------------------------------------------

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
        MouseButtonEvent(int button)
            : m_Button(button) {}

        int m_Button;
    };

    // -------------------------------------------------------------------------
    // MOUSE BUTTON IMPLEMENTATION HELPER
    // -------------------------------------------------------------------------

    /// @brief Intermediate template for Mouse Button events (Pressed/Released).
    template<typename Derived, EventType Type>
    class MouseButtonEventImpl : public MouseButtonEvent
    {
    public:
        static constexpr EventType GetStaticType() { return Type; }
        virtual EventType GetEventType() const override { return GetStaticType(); }
        virtual const char* GetName() const override { return Derived::Name; }

        MouseButtonEventImpl(int button) : MouseButtonEvent(button) {}
    };

    // -------------------------------------------------------------------------
    // MOUSE BUTTON PRESSED
    // -------------------------------------------------------------------------

    /// @brief Event triggered when a mouse button is clicked down.
    class MouseButtonPressedEvent : public MouseButtonEventImpl<MouseButtonPressedEvent, EventType::MouseButtonPressed>
    {
    public:
        static constexpr char Name[] = "MouseButtonPressed";

        MouseButtonPressedEvent(int button)
            : MouseButtonEventImpl(button) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_Button;
            return ss.str();
        }
    };

    // -------------------------------------------------------------------------
    // MOUSE BUTTON RELEASED
    // -------------------------------------------------------------------------

    /// @brief Event triggered when a mouse button is released.
    class MouseButtonReleasedEvent : public MouseButtonEventImpl<MouseButtonReleasedEvent, EventType::MouseButtonReleased>
    {
    public:
        static constexpr char Name[] = "MouseButtonReleased";

        MouseButtonReleasedEvent(int button)
            : MouseButtonEventImpl(button) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_Button;
            return ss.str();
        }
    };

}