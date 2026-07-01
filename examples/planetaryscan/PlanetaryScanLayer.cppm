export module Example.PlanetaryScanLayer;
import Nodens;
import std;

// -----------------------------------------------------------------------------
// Step 1: Define the Event class. This event will be used to communicate
// the results of the async task to the main thread.
// -----------------------------------------------------------------------------

export struct PlanetaryScanResultEvent
{
    static constexpr std::string_view Name = "PlanetaryScanResult";
    int m_ID{};
    float m_Distance{};
    float m_AtmosphereDensity{};
    float m_CalculationTime{};
};

// -----------------------------------------------------------------------------
// Step 2: Define the Layer class. This layer will be responsible for
// creating async tasks, handling their results, and rendering the UI.
// It is essentially the heart of the application, managing all the logic
// and data.
// -----------------------------------------------------------------------------

export class PlanetaryScanLayer : public Nodens::Layer
{
public:
    PlanetaryScanLayer();
    ~PlanetaryScanLayer() override = default;

    void OnAttach() override;
    void OnUpdate(Nodens::TimeStep ts) override;
    void OnImGuiRender(Nodens::TimeStep ts) override;

private:
    void AddResult(const PlanetaryScanResultEvent& e);

private:
    float m_TimePassed{};
    int m_ScanCounter{};

    std::mutex m_DataMutex;

    std::vector<float> m_GalaxyDistances{};
    std::vector<float> m_GalaxyDensities{};

    std::vector<float> m_LatencyHistory{};
    float m_MaxLatency{};
};
