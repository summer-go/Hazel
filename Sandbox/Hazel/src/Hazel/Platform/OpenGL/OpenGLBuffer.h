#pragma once
#include <stdint.h>
#include "Renderer/Buffer.h"

namespace Hazel {
    class OpenGLVertexBuffer : public VertexBuffer {
    public:
        OpenGLVertexBuffer(uint32_t size);
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        virtual ~OpenGLVertexBuffer();

        void Bind() const override;
        void Unbind() const override;

        virtual void SetData(const void* data, uint32_t size) override;

        const BufferLayout &GetLayout() const override {
            return m_Layout;
        };

        void SetLayout(const BufferLayout &layout) override {
            m_Layout = layout;
        };

    private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;
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