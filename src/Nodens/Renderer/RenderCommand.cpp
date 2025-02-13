#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "ndpch.h"

namespace Nodens {
RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}