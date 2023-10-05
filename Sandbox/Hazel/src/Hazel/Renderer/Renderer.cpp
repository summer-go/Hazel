//
// Created by xiatian05 on 2023/9/30.
//

#include "Renderer.h"
#include "./RenderCommand.h"

namespace Hazel{
    void Renderer::BeginScene() {

    }

    void Renderer::EndScene() {

    }

    void Renderer::Submit(const std::shared_ptr<VertexArray> &vertexArray) {
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}