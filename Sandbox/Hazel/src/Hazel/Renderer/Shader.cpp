//
// Created by xiatian05 on 2023/9/27.
//

#include "Shader.h"
#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>
#include "Log.h"
#include "Base.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <vector>

namespace Hazel {

    Shader *Shader::Create(const std::string& filePath) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return new OpenGLShader(filePath);

        }
        HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
    }

    Shader *Shader::Create(const std::string &vertexSrc, const std::string &fragmentSrc) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return new OpenGLShader(vertexSrc, fragmentSrc);

        }
        HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
    }
}