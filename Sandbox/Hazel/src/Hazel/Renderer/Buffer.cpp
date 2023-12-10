//
// Created by xiatian05 on 2023/9/30.
//

#include "Buffer.h"
#include "Renderer.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Log.h"
#include "base.h"

namespace Hazel {
    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None: HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLVertexBuffer>(size);
        }

        HZ_CORE_ASSERT(false, "Unknow RendererAPI!");
        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::Create(float *vertices, uint32_t size) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None: HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLVertexBuffer>(vertices, size);

        }

        HZ_CORE_ASSERT(false, "Unknow RendererAPI!");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t *indices, uint32_t size) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None: HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
                return nullptr;
            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLIndexBuffer>(indices, size);
        }

        HZ_CORE_ASSERT(false, "Unknow RendererAPI!");
        return nullptr;
    }


}