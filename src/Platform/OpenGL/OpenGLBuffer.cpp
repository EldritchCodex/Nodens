#include "OpenGLBuffer.h"

#include <glad/glad.h>

#include "ndpch.h"

namespace Nodens {
//=============================================================================
//======VertexBuffer===========================================================

OpenGLVertexBuffer::OpenGLVertexBuffer(float* data, uint32_t size)
    : m_DataCount(size) {
  glGenBuffers(1, &m_RendererID);
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &m_RendererID); }

void OpenGLVertexBuffer::Bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void OpenGLVertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void OpenGLVertexBuffer::CalculateVertexCount() {
  if (m_DataCount % m_Layout.GetComponentsCount() != 0)
    ND_CORE_WARN(
        "OpenGLVertexBuffer::CalculateVertexCount: DataCount not multiple of "
        "Layout's ComponentsCount!");
  m_VertexCount = m_DataCount / m_Layout.GetComponentsCount();
}

//=============================================================================
//======IndexBuffer============================================================

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
    : m_Count(count) {
  glGenBuffers(1, &m_RendererID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
               GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &m_RendererID); }

void OpenGLIndexBuffer::Bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void OpenGLIndexBuffer::Unbind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
}  // namespace Nodens
