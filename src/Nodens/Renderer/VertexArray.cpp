#include "VertexArray.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Renderer.h"
#include "ndpch.h"

namespace Nodens {
VertexArray* VertexArray::Create() {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
      ND_CORE_ASSERT(false, "RendererAPI::None is not supported!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return new OpenGLVertexArray();
  }

  ND_CORE_ASSERT(false, "Unknown RendererAPI");
  return nullptr;
}
}  // namespace Nodens