//
// Created by xiatian05 on 2023/10/24.
//

#include "Renderer2D.h"
#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Renderer2D::Renderer2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {
}

void Renderer2D::OnAttach() {
    m_SquareVA = Hazel::VertexArray::Create();
    float squareVertices[5 * 4] = {
            -0.5, -0.5f, 0.0f,
            0.5, -0.5f, 0.0f,
            0.5, 0.5f, 0.0f,
            -0.5, 0.5f, 0.0f,

    };

    Hazel::Ref<Hazel::VertexBuffer> squareVB;
    squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
    squareVB->SetLayout({
                                {Hazel::ShaderDataType::Float3, "a_Position"}
                        });
    m_SquareVA->AddVertexBuffer(squareVB);

    uint32_t squareIndices[6] = {0, 1, 2, 2,3, 0};
    Hazel::Ref<Hazel::IndexBuffer> squareIB;
    squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
    m_SquareVA->SetIndexBuffer(squareIB);

    m_FlatColorShader = Hazel::Shader::Create("../assets/shaders/FlatColor.glsl");
}

void Renderer2D::OnDetach() {

}

void Renderer2D::OnUpdate(Hazel::Timestep ts) {
    // Update
    m_CameraController.OnUpdate(ts);

    // Render
    Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0});
    Hazel::RenderCommand::Clear();

    Hazel::Renderer::BeginScene(m_CameraController.GetCamera());
    m_FlatColorShader->Bind();
    std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

    Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
    Hazel::Renderer::EndScene();
}

void Renderer2D::OnImGuiRender() {
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Renderer2D::OnEvent(Hazel::Event &e) {
    m_CameraController.OnEvent(e);
}
