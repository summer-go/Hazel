//
// Created by xiatian05 on 2023/10/4.
//

#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hazel{
    VertexArray* VertexArray::Create() {
        switch (Renderer::GetAPI()) {
            case RendererAPI::None: HZ_CORE_ASSERT(false, "RendererAPI::None is currently not suported!"); return nullptr;
            case RendererAPI::OpenGL: return new OpenGLVertexArray();
        }

        HZ_CORE_ASSERT(false, "Unknow RendererAPI!");
        return nullptr;
    }
}