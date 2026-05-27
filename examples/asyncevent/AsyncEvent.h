#pragma once
#include "nodens.h"

/// @brief Event for a planetary scan.
/// This event is dispatched when a planetary scan is requested.
/// It contains information about the scan, such as the planet ID, and is later filled with the scan results.
struct PlanetaryScanEvent : public Nodens::Event
{
    /// @brief Constructs a PlanetaryScanEvent.
    /// @param id The ID of the planet to scan.
    PlanetaryScanEvent(int id) : m_ID(id) {}

    int m_ID;

    // Result Data (Filled by Worker)
    float m_Distance          = 0.0f;
    float m_AtmosphereDensity = 0.0f;
    float m_CalculationTime   = 0.0f; // How long the thread took

    // Boilerplate
    /// @brief Gets the static type of the event.
    /// @return The static type of the event.
    static Nodens::EventType GetStaticType() { return (Nodens::EventType)0; }

    /// @brief Gets the event type.
    /// @return The event type.
    virtual Nodens::EventType GetEventType() const override { return GetStaticType(); }

    /// @brief Gets the name of the event.
    /// @return The name of the event.
    virtual const char* GetName() const override { return "PlanetaryScanEvent"; }

    /// @brief Gets the category flags of the event.
    /// @return The category flags of the event.
    virtual int GetCategoryFlags() const override { return 0; }
};