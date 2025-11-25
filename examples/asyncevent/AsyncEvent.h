#pragma once
#include <Nodens.h>

struct PlanetaryScanEvent : public Nodens::Event
{
    PlanetaryScanEvent(int id) : m_ID(id) {}

    int m_ID;

    // Result Data (Filled by Worker)
    float m_Distance          = 0.0f;
    float m_AtmosphereDensity = 0.0f;
    float m_CalculationTime   = 0.0f; // How long the thread took

    // Boilerplate
    static Nodens::EventType  GetStaticType() { return (Nodens::EventType)0; }
    virtual Nodens::EventType GetEventType() const override { return GetStaticType(); }
    virtual const char*       GetName() const override { return "PlanetaryScanEvent"; }
    virtual int               GetCategoryFlags() const override { return 0; }
};