//
// Created by xiatian05 on 2023/10/4.
//

#include "OpenGLVertexArray.h"
#include "Core.h"
#include "Debug/Instrumentor.h"

#include <glad/glad.h>

namespace Hazel{
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:         return GL_FLOAT;
            case ShaderDataType::Float2:        return GL_FLOAT;
            case ShaderDataType::Float3:        return GL_FLOAT;
            case ShaderDataType::Float4:        return GL_FLOAT;
            case ShaderDataType::Mat3:          return GL_FLOAT;
            case ShaderDataType::Mat4:          return GL_FLOAT;
            case ShaderDataType::Int:           return GL_INT;
            case ShaderDataType::Int2:          return GL_INT;
            case ShaderDataType::Int3:          return GL_INT;
            case ShaderDataType::Int4:          return GL_INT;
            case ShaderDataType::Bool:          return GL_BOOL;
        }
        HZ_CORE_ASSERT(false, "Unknow ShaderDataType!");
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray() {
        HZ_PROFILE_FUNCTION();

        glGenVertexArrays(1, &m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray() {
        HZ_PROFILE_FUNCTION();

        glDeleteVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::Bind() const {
        HZ_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::Unbind() const {
        HZ_PROFILE_FUNCTION();

        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) {
        HZ_PROFILE_FUNCTION();

        HZ_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout");

        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();

        uint32_t index = 0;
        const auto& layout = vertexBuffer->GetLayout();
        for (const auto& element : layout) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
                                  element.GetComponentCount(),
                                  ShaderDataTypeToOpenGLBaseType(element.Type),
                                  element.Normallized ? GL_TRUE : GL_FALSE,
                                  layout.GetStride(),
                                  (const void*)element.Offset);
            index++;
        }
        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(Ref<IndexBuffer> &indexBuffer) {
        HZ_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }
}