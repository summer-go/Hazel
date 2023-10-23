#pragma once

#include <glm/glm.hpp>
#include "Renderer/RendererAPI.h"
#include "VertexArray.h"

namespace Hazel {
    class RenderCommand {
    public:
        inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
            s_RendererAPI->SetViewport(x, y, width, height);
        }
        static inline void SetClearColor(const glm::vec4& color) {
            s_RendererAPI->SetClearColor(color);
        } ;
        static inline void Clear() {
            s_RendererAPI->Clear();
        }
        static inline void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
            s_RendererAPI->DrawIndexed(vertexArray);
        }

        static void Init() {
            s_RendererAPI->Init();
        };

    private:
        static RendererAPI* s_RendererAPI;
    };
}