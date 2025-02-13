#pragma once

#include <Nodens/Events/KeyEvent.h>
#include <Nodens/Events/MouseEvent.h>

#include "Nodens/Events/ApplicationEvent.h"
#include "Nodens/Layer.h"

namespace Nodens {
class ImGuiLayer : public Layer {
 public:
  ImGuiLayer();
  ~ImGuiLayer();

  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnImGuiRender(TimeStep ts) override;

  void Begin();
  void End();

 private:
  float m_Time = 0.0f;
  //
};
}  // namespace Nodens