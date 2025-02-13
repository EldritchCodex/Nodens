#pragma once

#include "RendererAPI.h"

namespace Nodens {

class RenderCommand {
 public:
  inline static void SetFrontFaceOrientation(
      const RendererProps::FrontFaceOrientation orientation) {
    s_RendererAPI->SetFrontFaceOrientation(orientation);
  }
  inline static void SetPolygonMode(const RendererProps::PolygonMode mode) {
    s_RendererAPI->SetPolygonMode(mode);
  }
  inline static void SetClearColor(const glm::vec4& color) {
    s_RendererAPI->SetClearColor(color);
  }
  inline static void Clear() { s_RendererAPI->Clear(); }

  static void SetLineWdith(const float width) {
    s_RendererAPI->SetLineWidth(width);
  }

  static void SetPointSize(const float size) {
    s_RendererAPI->SetPointSize(size);
  }

  static void DrawIndexedLines(const Ref<VertexArray>& vertexArray) {
    s_RendererAPI->DrawIndexedLines(vertexArray);
  }
  static void DrawIndexedPoints(const Ref<VertexArray>& vertexArray) {
    s_RendererAPI->DrawIndexedPoints(vertexArray);
  }
  static void DrawIndexed(const Ref<VertexArray>& vertexArray) {
    s_RendererAPI->DrawIndexed(vertexArray);
  }
  static void DrawArrays(const Ref<VertexArray>& vertexArray) {
    s_RendererAPI->DrawArrays(vertexArray);
  }

 private:
  static RendererAPI* s_RendererAPI;
};
}  // namespace Nodens