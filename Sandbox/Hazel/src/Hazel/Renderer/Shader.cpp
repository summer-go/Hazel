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

#include <map>

namespace Hazel {

    Ref<Shader> Shader::Create(const std::string& filePath) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLShader>(filePath);

        }
        HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
    }

    Ref<Shader> Shader::Create(const std::string& name, const std::string &vertexSrc, const std::string &fragmentSrc) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None:
                HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);

        }
        HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
    }

    void ShaderLibrary::Add(const std::string &name, const Ref<Shader> &shader) {
        HZ_CORE_ASSERT(!Exists(name), "Shader already exists!");
        m_Shaders[name] = shader;
    }

    void ShaderLibrary::Add(const Ref<Shader> &shader) {
        auto& name = shader->GetName();
        HZ_CORE_ASSERT(!Exists(name), "Shader already exists!");
        Add(name, shader);
    }

    Ref<Shader> ShaderLibrary::Load(const std::string &filePath) {
        auto shader = Shader::Create(filePath);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string &name, const std::string &filePath) {
        auto shader = Shader::Create(filePath);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Get(const std::string &name) const {
        HZ_CORE_ASSERT(Exists(name), "Share not found!");
        return m_Shaders.at(name);
    }

    bool ShaderLibrary::Exists(const std::string &name) const {
        return m_Shaders.find(name) != m_Shaders.end();
    }
}