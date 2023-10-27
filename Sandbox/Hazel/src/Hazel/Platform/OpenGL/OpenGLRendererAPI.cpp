//
// Created by xiatian05 on 2023/10/6.
//

#include "OpenGLRendererAPI.h"
#include <glad/glad.h>
#include "Core.h"

namespace Hazel{

    void OpenGLRendererAPI::SetClearColor(const glm::vec4 &color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray> &vertexArray) {
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLRendererAPI::Init() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        // mac 屏幕上有问题，不知道为啥
        glViewport(x, y, width, height);
    }
}