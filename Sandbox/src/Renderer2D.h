#pragma once

#include "Hazel.h"

class Renderer2D : public Hazel::Layer {
public:
    Renderer2D();

    ~Renderer2D() override = default;

    void OnAttach() override;

    void OnDetach() override;

    void OnUpdate(Hazel::Timestep ts) override;

    void OnImGuiRender() override;

    void OnEvent(Hazel::Event &e) override;

private:
    Hazel::OrthographicCameraController m_CameraController;

    // Temp
    Hazel::Ref<Hazel::VertexArray> m_SquareVA;
    Hazel::Ref<Hazel::Shader> m_FlatColorShader;

    glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};
};

