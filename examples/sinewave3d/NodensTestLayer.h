#pragma once

#include <fstream>
#include <list>

#include "nodens.h"  // Nodens Engine lib

class NodensTestLayer : public Nodens::Layer {
 public:
  // Nodens Engine Functions
  // =========================================================================================
  /**
   *   \brief The constructor is called only once when the layer is created in
   * the stack.
   */
  NodensTestLayer();

  /**
   *   \brief This function is called once per frame.
   */
  void OnUpdate(Nodens::TimeStep ts) override;

  /**
   *   \brief This function is called once per frame after OnUpdate.
   */
  void OnImGuiRender(Nodens::TimeStep ts) override;

  void OnEvent(Nodens::Event& event) override;
  bool OnWindowResizeEvent(Nodens::WindowResizeEvent& event);

 private:
  // Template Variables
  // ==============================================================================================
  float xs[1001], ys[1001];
  float xs3d[1001], ys3d[1001], zs3d[1001];
};
