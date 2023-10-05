//
// Created by xiatian05 on 2023/10/4.
//

#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "RendererAPI.h"

namespace Hazel{
    VertexArray* VertexArray::Create() {
        switch (RendererAPI::GetAPI()) {
            case RendererAPI::API::None: HZ_CORE_ASSERT(false, "RendererAPI::None is currently not suported!"); return nullptr;
            case RendererAPI::API::OpenGL: return new OpenGLVertexArray();
        }

        HZ_CORE_ASSERT(false, "Unknow RendererAPI!");
        return nullptr;
    }
}