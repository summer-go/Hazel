//
// Created by xiatian05 on 2023/9/30.
//

#include "Renderer.h"
#include "./RenderCommand.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel{
    Renderer::SceneDatta* Renderer::s_SceneData = new Renderer::SceneDatta;
    void Renderer::BeginScene(OrthographicCamera(& camera)) {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene() {

    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray> &vertexArray, const glm::mat4& transform) {
        shader->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}