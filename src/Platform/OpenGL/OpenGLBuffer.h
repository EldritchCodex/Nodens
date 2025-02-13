#pragma once

#include "Nodens/Renderer/Buffer.h"

namespace Nodens {
class OpenGLVertexBuffer : public VertexBuffer {
 public:
  OpenGLVertexBuffer(float* data, uint32_t size);
  virtual ~OpenGLVertexBuffer();

  virtual void Bind() const override;
  virtual void Unbind() const override;

  virtual const BufferLayout& GetLayout() const override { return m_Layout; }
  virtual void SetLayout(const BufferLayout& layout) override {
    m_Layout = layout;
    CalculateVertexCount();
  }

  virtual const uint32_t GetVertexCount() const override {
    return m_VertexCount;
  }

 private:
  void CalculateVertexCount();

 private:
  uint32_t m_RendererID;
  BufferLayout m_Layout;
  uint32_t m_DataCount;
  uint32_t m_VertexCount;  // Number of vertices
};

class OpenGLIndexBuffer : public IndexBuffer {
 public:
  OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
  virtual ~OpenGLIndexBuffer();

  virtual void Bind() const override;
  virtual void Unbind() const override;

  virtual uint32_t GetCount() const { return m_Count; };

 private:
  uint32_t m_RendererID;
  uint32_t m_Count;
};
}  // namespace Nodens