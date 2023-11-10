#pragma once

#include "RendererAPI.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace Hazel {


    class Renderer {
    public:
        static void Init();
        static void Shutdown();
        static void BeginScene(OrthographicCamera& camera);
        static void EndScene();
        static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray,
                           const glm::mat4& transform = glm::mat4(1.0f));
        static void OnWindowResize(uint32_t width, uint32_t height);

        inline static RendererAPI::API GetAPI() {
            return RendererAPI::GetAPI();
        }

        struct SceneData {
            glm::mat4 ViewProjectionMatrix;
        };

        static SceneData* s_SceneData;
    };
}
