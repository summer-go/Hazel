#pragma once

#include "Renderer/RendererAPI.h"
namespace Hazel {
    class OpenGLRendererAPI : public RendererAPI{
        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;
        void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
        void Init() override;
    };
}