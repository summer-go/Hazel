//
// Created by xiatian05 on 2023/10/11.
//

#include "Texture.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Hazel {

    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None: HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(width, height);
        }

        HZ_CORE_ASSERT(false, "Unknow RendererAPI!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string &path) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None: HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);
        }

        HZ_CORE_ASSERT(false, "Unknow RendererAPI!");
        return nullptr;
    }
}