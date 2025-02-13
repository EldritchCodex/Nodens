#pragma once

#include "Nodens/Renderer/RendererAPI.h"
#include "glad/glad.h"

namespace Nodens {
class OpenGLRendererAPI : public RendererAPI {
  virtual void SetFrontFaceOrientation(
      const RendererProps::FrontFaceOrientation orientation) override;
  virtual void SetPolygonMode(const RendererProps::PolygonMode mode) override;
  virtual void SetClearColor(const glm::vec4& color) override;

  virtual void SetLineWidth(const float width) override;
  virtual void SetPointSize(const float size) override;

  virtual void DrawIndexedLines(const Ref<VertexArray>& vertexArray) override;
  virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
  virtual void DrawArrays(const Ref<VertexArray>& vertexArray) override;
  virtual void DrawIndexedPoints(const Ref<VertexArray>& vertexArray) override;

  virtual void Clear() override;
};

}  // namespace Nodens
