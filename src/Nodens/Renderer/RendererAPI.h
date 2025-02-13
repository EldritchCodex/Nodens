#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"

namespace Nodens {

struct RendererProps {
  enum class PolygonMode { Fill = 0, Wireframe, Point };

  enum class FrontFaceOrientation { CounterClockwise = 0, Clockwise };
};

class RendererAPI {
 public:
  enum class API { None = 0, OpenGL = 1 };

 public:
  virtual void SetFrontFaceOrientation(
      const RendererProps::FrontFaceOrientation orientation) = 0;
  virtual void SetPolygonMode(const RendererProps::PolygonMode mode) = 0;
  virtual void SetClearColor(const glm::vec4& color) = 0;
  virtual void Clear() = 0;

  virtual void SetLineWidth(const float width) = 0;
  virtual void SetPointSize(const float size) = 0;

  virtual void DrawIndexedLines(const Ref<VertexArray>& vertexArray) = 0;
  virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;
  virtual void DrawArrays(const Ref<VertexArray>& vertexArray) = 0;
  virtual void DrawIndexedPoints(const Ref<VertexArray>& vertexArray) = 0;

  static inline API GetAPI() { return s_API; }

 private:
  static API s_API;
};

}  // namespace Nodens
