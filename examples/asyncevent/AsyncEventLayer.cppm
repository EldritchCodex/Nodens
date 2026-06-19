export module Example.AsyncEventLayer;

import Nodens;
import std;

// -----------------------------------------------------------------------------
// Step 1: Define the Event class. This event will be used to communicate
// the results of the async task to the main thread.
// -----------------------------------------------------------------------------

export struct PlanetaryScanEvent : public Nodens::Event
{
    PlanetaryScanEvent(int id) : m_ID(id)
    {
    }

    int m_ID{};

    float m_Distance{};
    float m_AtmosphereDensity{};
    float m_CalculationTime{};

    static Nodens::EventType GetStaticType()
    {
        return (Nodens::EventType)0;
    }
    Nodens::EventType GetEventType() const override
    {
        return GetStaticType();
    }
    const char* GetName() const override
    {
        return "PlanetaryScanEvent";
    }
    int GetCategoryFlags() const override
    {
        return 0;
    }
};

// -----------------------------------------------------------------------------
// Step 2: Define the Layer class. This layer will be responsible for
// creating async tasks, handling their results, and rendering the UI.
// It is essentially the heart of the application, managing all the logic
// and data.
// -----------------------------------------------------------------------------

export class AsyncEventLayer : public Nodens::Layer
{
public:
    AsyncEventLayer();
    ~AsyncEventLayer() override = default;

    void OnAttach() override;
    void OnUpdate(Nodens::TimeStep ts) override;
    void OnImGuiRender(Nodens::TimeStep ts) override;

private:
    void AddResult(const PlanetaryScanEvent& e);

private:
    float m_TimePassed{};
    int m_ScanCounter{};

    std::mutex m_DataMutex;

    std::vector<float> m_GalaxyDistances{};
    std::vector<float> m_GalaxyDensities{};

    std::vector<float> m_LatencyHistory{};
    float m_MaxLatency{};
};
