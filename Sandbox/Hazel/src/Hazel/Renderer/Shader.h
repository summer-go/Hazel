#pragma once
#include <string>
#include <glm/glm.hpp>
namespace Hazel {
    class Shader {
    public:
        ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static Shader* Create(const std::string& filePath);
        static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
    };
}