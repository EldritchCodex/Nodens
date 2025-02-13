#pragma once

#include "Nodens/Core.h"
#include "Nodens/Core/TimeStep.h"
#include "Nodens/Events/Event.h"

namespace Nodens {

class Layer {
 public:
  Layer(const std::string& name = "Layer");
  virtual ~Layer();

  virtual void OnAttach() {}
  virtual void OnDetach() {}
  virtual void OnUpdate(TimeStep ts) {}
  virtual void OnImGuiRender(TimeStep ts) {}
  virtual void OnEvent(Event& event) {}

  inline const std::string& GetName() const { return m_DebugName; }

 protected:
  std::string m_DebugName;
};
}  // namespace Nodens
