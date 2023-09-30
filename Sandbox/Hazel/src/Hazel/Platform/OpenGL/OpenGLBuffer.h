#pragma once
#include <stdint.h>
#include "Renderer/Buffer.h"

namespace Hazel {
    class OpenGLVertexBuffer : public VertexBuffer {
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        virtual ~OpenGLVertexBuffer();

        void Bind() const override;
        void Unbind() const override;

    private:
        uint32_t m_RendererID;
    };

    class OpenGLIndexBuffer : public IndexBuffer {
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);

        virtual ~OpenGLIndexBuffer();

        void Bind() const override;

        void Unbind() const override;

        uint32_t GetCount() const override {return m_Count;}

    private:
        uint32_t m_RendererID;
        uint32_t m_Count;
    };
}