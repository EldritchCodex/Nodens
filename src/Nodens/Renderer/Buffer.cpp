#include "Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Renderer.h"
#include "ndpch.h"

namespace Nodens {
VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
      ND_CORE_ASSERT(false, "RendererAPI::None is not supported!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return new OpenGLVertexBuffer(vertices, size);
  }

  ND_CORE_ASSERT(false, "Unknown RendererAPI");
  return nullptr;
}

IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
      ND_CORE_ASSERT(false, "RendererAPI::None is not supported!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return new OpenGLIndexBuffer(indices, size);
  }

  ND_CORE_ASSERT(false, "Unknown RendererAPI");
  return nullptr;
}
}  // namespace Nodens