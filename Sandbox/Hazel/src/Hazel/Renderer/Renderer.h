#pragma once

#include "RendererAPI.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace Hazel {


    class Renderer {
    public:

        static void BeginScene(OrthographicCamera& camera);
        static void EndScene();
        static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray,
                           const glm::mat4& transform = glm::mat4(1.0f));

        inline static RendererAPI::API GetAPI() {
            return RendererAPI::GetAPI();
        }

        struct SceneDatta {
            glm::mat4 ViewProjectionMatrix;
        };

        static SceneDatta* s_SceneData;
    };
}
