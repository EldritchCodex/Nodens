#pragma once

#include <Nodens.h>
#include <Nodens/Events/AsyncEventBus.h>
#include <implot.h>  // <--- Add this

#include <mutex>
#include <sstream>
#include <vector>

// ==================================================================
// 1. DEFINE GAMEPLAY EVENT
// ==================================================================
struct PlanetaryScanEvent : public Nodens::Event {
  PlanetaryScanEvent(int id) : m_ID(id) {}

  int m_ID;

  // Result Data (Filled by Worker)
  float m_Distance = 0.0f;
  float m_AtmosphereDensity = 0.0f;
  float m_CalculationTime = 0.0f;  // How long the thread took

  // Boilerplate
  static Nodens::EventType GetStaticType() { return (Nodens::EventType)0; }
  virtual Nodens::EventType GetEventType() const override {
    return GetStaticType();
  }
  virtual const char* GetName() const override { return "PlanetaryScanEvent"; }
  virtual int GetCategoryFlags() const override { return 0; }
};

class AsyncEventLayer : public Nodens::Layer {
 public:
  AsyncEventLayer();
  virtual ~AsyncEventLayer() = default;

  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnUpdate(Nodens::TimeStep ts) override;
  virtual void OnImGuiRender(Nodens::TimeStep ts) override;

 private:
  void AddResult(const PlanetaryScanEvent& e);

 private:
  // Simulation State
  float m_TimePassed = 0.0f;
  int m_ScanCounter = 0;

  // ==============================================================
  // VISUALIZATION DATA
  // ==============================================================
  std::mutex m_DataMutex;

  // Plot 1: Galaxy Composition (Scatter Plot)
  std::vector<float> m_GalaxyDistances;  // X-Axis
  std::vector<float> m_GalaxyDensities;  // Y-Axis

  // Plot 2: Performance Stats (Line/Histogram)
  std::vector<float> m_LatencyHistory;
  float m_MaxLatency = 0.0f;
};