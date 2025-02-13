#pragma once

namespace Nodens {
class TimeStep {
 public:
  TimeStep(float time = 0.0f) : m_Time(time) {}

  operator float() const { return m_Time; }
  float operator+(const float a) const { return m_Time + a; }

  float GetSeconds() const { return m_Time; }
  float GetMilliseconds() const { return m_Time * 1000; }

 private:
  float m_Time;
};
}  // namespace Nodens