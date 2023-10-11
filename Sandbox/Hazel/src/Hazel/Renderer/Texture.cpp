//
// Created by xiatian05 on 2023/10/11.
//

#include "Texture.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Hazel {

    Ref<Texture2D> Texture2D::Create(const std::string &path) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None: HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
        }

        HZ_CORE_ASSERT(false, "Unknow RendererAPI!");
        return nullptr;
    }
}