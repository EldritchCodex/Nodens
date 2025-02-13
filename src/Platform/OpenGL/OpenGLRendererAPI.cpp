#include "OpenGLRendererAPI.h"

#include "ndpch.h"

namespace Nodens {
void OpenGLRendererAPI::SetFrontFaceOrientation(
    const RendererProps::FrontFaceOrientation orientation) {
  if (orientation == RendererProps::FrontFaceOrientation::CounterClockwise)
    glFrontFace(GL_CCW);
  else if (orientation == RendererProps::FrontFaceOrientation::Clockwise)
    glFrontFace(GL_CW);
}

void OpenGLRendererAPI::SetPolygonMode(const RendererProps::PolygonMode mode) {
  switch (mode) {
    case RendererProps::PolygonMode::Wireframe:
      glDisable(GL_CULL_FACE);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      break;
    case RendererProps::PolygonMode::Fill:
      glEnable(GL_CULL_FACE);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;
    case RendererProps::PolygonMode::Point:
      glDisable(GL_CULL_FACE);
      glEnable(GL_POINT_SMOOTH);
      glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
      break;
    default:
      break;
  }
}

void OpenGLRendererAPI::SetLineWidth(const float width) { glLineWidth(width); }

void OpenGLRendererAPI::SetPointSize(const float size) { glPointSize(size); }

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexedPoints(const Ref<VertexArray>& vertexArray) {
  glDrawElements(GL_POINTS, vertexArray->GetIndexBuffer()->GetCount(),
                 GL_UNSIGNED_INT, nullptr);
}

void OpenGLRendererAPI::DrawIndexedLines(const Ref<VertexArray>& vertexArray) {
  glDrawElements(GL_LINE_STRIP, vertexArray->GetIndexBuffer()->GetCount(),
                 GL_UNSIGNED_INT, nullptr);
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray) {
  glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(),
                 GL_UNSIGNED_INT, nullptr);
}

void OpenGLRendererAPI::DrawArrays(const Ref<VertexArray>& vertexArray) {
  glDrawArrays(GL_TRIANGLES, 0,
               vertexArray->GetVertexBuffers()[0]->GetVertexCount());
}
}  // namespace Nodens
