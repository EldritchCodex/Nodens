export module Example.AsyncEvent;

import Nodens.Events;

export struct PlanetaryScanEvent : public Nodens::Event
{
    PlanetaryScanEvent(int id) : m_ID(id) {}

    int m_ID;

    float m_Distance          = 0.0f;
    float m_AtmosphereDensity = 0.0f;
    float m_CalculationTime   = 0.0f;

    static Nodens::EventType GetStaticType() { return (Nodens::EventType)0; }
    Nodens::EventType GetEventType() const override { return GetStaticType(); }
    const char* GetName() const override { return "PlanetaryScanEvent"; }
    int GetCategoryFlags() const override { return 0; }
};
