#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer.h"
#include "ndpch.h"

namespace Nodens {
Shader* Shader::Create(const std::string& vertexSrc,
                       const std::string& fragmentSrc) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
      ND_CORE_ASSERT(false, "RendererAPI::None is not supported!");
      return nullptr;
    case RendererAPI::API::OpenGL:
      return new OpenGLShader(vertexSrc, fragmentSrc);
  }

  ND_CORE_ASSERT(false, "Unknown RendererAPI");
  return nullptr;
}
}  // namespace Nodens