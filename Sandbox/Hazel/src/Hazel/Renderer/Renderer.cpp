//
// Created by xiatian05 on 2023/9/30.
//

#include "Renderer.h"
#include "./RenderCommand.h"

namespace Hazel{
    Renderer::SceneDatta* Renderer::m_SceneData = new Renderer::SceneDatta;
    void Renderer::BeginScene(OrthographicCamera(& camera)) {
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene() {

    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray> &vertexArray) {
        shader->Bind();
        shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}