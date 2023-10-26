//
// Created by xiatian05 on 2023/10/24.
//

#include "Sandbox2D.h"
#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true) {
}

void Sandbox2D::OnAttach() {
}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnUpdate(Hazel::Timestep ts) {
    // Update
    m_CameraController.OnUpdate(ts);

    // Render
    Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0});
    Hazel::RenderCommand::Clear();

    Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
    Hazel::Renderer2D::DrawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
    Hazel::Renderer2D::DrawQuad({0.5f, -0.5f}, {0.5f, 0.5f}, {0.2f, 0.3f, 0.8f, 1.0f});
    Hazel::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender() {
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event &e) {
    m_CameraController.OnEvent(e);
}
