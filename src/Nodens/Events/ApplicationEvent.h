#pragma once

#include "Nodens/Events/Event.h"
#include <sstream>

namespace Nodens {

    // -------------------------------------------------------------------------
    // WINDOW RESIZE
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
        WindowResizeEvent(unsigned int width, unsigned int height)
            : m_Width(width), m_Height(height) {}

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

    // -------------------------------------------------------------------------
    // WINDOW CLOSE
    // -------------------------------------------------------------------------

    /// @brief Event triggered when the user attempts to close the window.
    /// @details This is usually dispatched before the application actually shuts down,
    /// allowing systems to save state or cancel the close.
    class WindowCloseEvent : public EventImpl<WindowCloseEvent, EventType::WindowClose>
    {
    public:
        static constexpr char Name[] = "WindowClose";
        static constexpr int Category = EventCategoryApplication;

        WindowCloseEvent() = default;
    };

    // -------------------------------------------------------------------------
    // APP TICK
    // -------------------------------------------------------------------------

    /// @brief Event triggered every fixed simulation step.
    /// @details Useful for physics or fixed-timestep logic logic.
    class AppTickEvent : public EventImpl<AppTickEvent, EventType::AppTick>
    {
    public:
        static constexpr char Name[] = "AppTick";
        static constexpr int Category = EventCategoryApplication;

        AppTickEvent() = default;
    };

    // -------------------------------------------------------------------------
    // APP UPDATE
    // -------------------------------------------------------------------------

    /// @brief Event triggered once per frame update.
    /// @details Use this for variable time-step logic (Input polling, Camera movement).
    class AppUpdateEvent : public EventImpl<AppUpdateEvent, EventType::AppUpdate>
    {
    public:
        static constexpr char Name[] = "AppUpdate";
        static constexpr int Category = EventCategoryApplication;

        AppUpdateEvent() = default;
    };

    // -------------------------------------------------------------------------
    // APP RENDER
    // -------------------------------------------------------------------------

    /// @brief Event triggered when the application is ready to render.
    /// @details This is often used to synchronize ImGui rendering or custom draw passes.
    class AppRenderEvent : public EventImpl<AppRenderEvent, EventType::AppRender>
    {
    public:
        static constexpr char Name[] = "AppRender";
        static constexpr int Category = EventCategoryApplication;

        AppRenderEvent() = default;
    };

}