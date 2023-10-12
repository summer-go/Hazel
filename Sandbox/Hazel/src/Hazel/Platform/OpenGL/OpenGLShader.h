#pragma once

#include "Renderer/Shader.h"

// TODO:REMOVE!
typedef unsigned int GLenum;

namespace Hazel {
    class OpenGLShader : public Shader {
    public:
        OpenGLShader(const std::string& filePath);
        OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~OpenGLShader();
        void Bind() const override;
        void Unbind() const override;

        void UploadUniformInt(const std::string& name, int value);

        void UploadUniformFloat(const std::string& name, float value);
        void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
        void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
        void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

        void UploadUniformMat3(const std::string& name, const glm::mat3 value);
        void UploadUniformMat4(const std::string& name, const glm::mat4 value);

    private:
        std::string ReadFile(const std::string& filePath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
    private:
        uint32_t m_RendererID;
    };
}
